/* This file implements some of the functions described in
 * tads2/osifc.h.  We don't need to implement them all, as most of them
 * are provided by tads2/osnoui.c and tads2/osgen3.c.
 *
 * This file only implements the functions that use curses routines.
 * Functions that don't need curses are implemented in osportable.cc.
 */
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//#include <assert.h>
#include <stddef.h>

#include "os.h"
extern "C" {
#include "osgen.h"
}

#include "frobtadsapp.h"
#include "frobcurses.h"


/* We define this because osgen3 needs it.
 */
int status_mode = 0;

/* We store the UNDO record limit here.  This allows us to change it
 * at application start-up rather than having to hard-code it at compile
 * time.
 *
 * We initialize it to -1 for a good reason; if the TADS3 base code
 * changes at some point and tries to create its internal UNDO-buffer(s)
 * before we get a chance to put the actual value here, we'll get a
 * segmentation fault (you cannot allocate negative sizes).  This will
 * be an indication that we must redesign the way we allow the user to
 * change the UNDO-size.
 */
int frobVmUndoMaxRecords = -1;


/* Read a character from the keyboard.
 *
 * This routine is not needed by the VM.  It's commented out rather than
 * left in as a dummy, so that we'll get a linker-error if the VM
 * decides to use it someday; that way we'll know that we must implement
 * it.  A dummy would result in a working build but non-working runtime.
 */
/*
#ifdef RUNTIME
int os_getc(void)
{
}
#endif
*/


/* Uses os_getc_raw() semantics, but with a timeout.
 *
 * If 'timeout' is negative, then the routine behaves exactly like
 * os_getc_raw().  If 'timeout' is positive, then we only wait for a key
 * for 'timeout' milliseconds.  It 'timeout' is 0, we don't wait for a
 * key but still return one if there was one in the system's input buffer.
 * If the operation times out before a key has been pressed, we return 0
 * and set 'timedOut' to true.  If a key is pressed before the timeout is
 * reached, we return the same as os_getc_raw() and set 'timedOut' to
 * false.
 *
 * 'showCursor' enables/disables the cursor while waiting for input.
 */
static int
timedGetcRaw( bool showCursor, int timeout = -1, bool* timedOut = 0)
{
    // If `done' is false, it means that we have been previously
    // called and returned 0, so this time we should return the
    // extended key-code we stored last time in `extKey'.
    static bool done = true;
    static int extKey;

    if (not done) {
        // We have a pending return from our last call.  Prepare to do a
        // normal read on our next call and return the pending result.
        done = true;
        return extKey;
    }

    // Read a character.
    int c = globalApp->getRawChar(showCursor, timeout);
    extKey = 0;

    if (c == ERR) {
        if (timeout > -1) {
            // The operation timed out.
            if (timedOut != 0) *timedOut = true;
            return 0;
        }
        // Paranoia (ERR is only returned to indicate that the operation
        // timed out).  Something else happened.  Prepare to return an EOF
        // on our next call.
        extKey = CMD_EOF;
    }

    // If a timeout was specified and the caller wants to know, report that
    // no timeout occured.
    if (timeout > -1 and timedOut != 0) *timedOut = false;
        
    switch (c) {
      // Paranoia.
      // ERR should always be 0, and therefore already handled.  Anyway, we
      // explicitly check for 0 here just in case ERR != 0.
      case 0:         extKey = CMD_EOF; break;
      // A Tab is not an extended character, but Tads requires that it is
      // handled as one.
      case '\t':      extKey = CMD_TAB; break;
      case '\n':
      case '\r':
      case KEY_ENTER: return 13;
      case KEY_DOWN:  extKey = CMD_DOWN; break;
      case KEY_UP:    extKey = CMD_UP; break;
      case KEY_LEFT:  extKey = CMD_LEFT; break;
      case KEY_RIGHT: extKey = CMD_RIGHT; break;
      case KEY_HOME:  extKey = CMD_HOME; break;
      // We don't return '\b' because of paranoia; some systems might not
      // use ASCII code 8 for '\b'.
      case '\b':
      case KEY_BACKSPACE: return 8;
      case KEY_F(1):  extKey = CMD_F1; break;
      case KEY_F(2):  extKey = CMD_F2; break;
      case KEY_F(3):  extKey = CMD_F3; break;
      case KEY_F(4):  extKey = CMD_F4; break;
      case KEY_F(5):  extKey = CMD_F5; break;
      case KEY_F(6):  extKey = CMD_F6; break;
      case KEY_F(7):  extKey = CMD_F7; break;
      case KEY_F(8):  extKey = CMD_F8; break;
      case KEY_F(9):  extKey = CMD_F9; break;
      case KEY_F(10): extKey = CMD_F10; break;
      case KEY_DL:    extKey = CMD_KILL; break;
      case KEY_DC:    extKey = CMD_DEL; break;
      case KEY_EOL:   extKey = CMD_DEOL; break;
      case KEY_NPAGE: extKey = CMD_PGDN; break;
      case KEY_PPAGE: extKey = CMD_PGUP; break;
      case KEY_END:   extKey = CMD_END; break;
      default:
        // TODO: This assumes that the system only returns unsigned
        // characters for "normal" inputs.
        if (c < 0 or c > 255) {
            // Who knows?  Report a space so that there's at least some
            // feedback.
            return ' ';
        }
    }

    if (extKey == 0) {
        // It's a normal ASCII code (this includes Escape, which has code
        // 27).
        return c;
    }

    // Prepare to return the extended key-code on our next call.
    done = false;
    return 0;
}


/* Flush any buffered display output.
 */
void os_flush( void )
{
    globalApp->flush();
}


/* Read a character from the keyboard and return the low-level,
 * untranslated key code whenever possible.
 */
int
os_getc_raw( void )
{
    // Just read a character without a timeout and return it.
    return timedGetcRaw(true);
}


/* Wait for a character to become available from the keyboard.
 *
 * We only implement this if we are building the interpreter.
 */
#ifdef RUNTIME
void
os_waitc( void )
{
    // Just read a character with no timeout and ignore its value.
    // Don't show a cursor.
    globalApp->getRawChar(false, -1);
}
#endif


/* Get an input event.
 */
int
os_get_event( unsigned long timeout, int use_timeout, os_event_info_t* info )
{
    int res;
    bool timedOut = false;

    res = timedGetcRaw(true, use_timeout ? timeout : -1, &timedOut);

    // If the timeout expired, tell TADS about it.
    if (use_timeout and timedOut) return OS_EVT_TIMEOUT;

    if (res == 0) {
        // It was an extended character; call again to get the
        // extended code.
        info->key[0] = 0;
        info->key[1] = timedGetcRaw(true);
    } else {
        // A normal character.  Return it as is.
        info->key[0] = res;
        info->key[1] = 0;
    }
    // Tell the caller it was an key-event.
    return OS_EVT_KEY;
}


/* Sleep for a while.
 */
void
os_sleep_ms( long delay_in_milliseconds )
{
    // Tell TADS to redraw the screen if needed.
    osssb_redraw_if_needed();
    globalApp->sleep(delay_in_milliseconds);
}


/* Terminate.
 *
 * The TADS 2 VM (not TADS 3) calls this routine when the user types
 * $$ABEND in a game.  The intention is to provide an emergency-exit in
 * case the game has fucked up somehow.  We don't do anything here, as
 * TADS 2 falls-back to a more sane exit-sequence when this function
 * actually returns.
 */
void
os_term( int )
{
}


/* Pause prior to exit, if desired.
 */
void
os_expause( void )
{
    if (globalApp->options.exitPause) {
        // Exit-pause is enabled.  Tell the user, flush any
        // pending output and wait for a key.
        os_printz("[Hit any key to exit.]");
        os_flush();
        os_waitc();
    }
}


/* Check for user break.
 *
 * TODO: Find out if we need this.
 */
int
os_break( void )
{
    return 0;
}


/* Initialize the time zone.
 *
 * TODO: Find out if this can be empty on all Unices.
 */
//void
//os_tzset( void )
//{
//}
