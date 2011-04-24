/* The FrobTadsWindow class provides an easy to use interface to the
 * curses WINDOW* structure.  It's basically a wrapper around the curses
 * routines, tailored somewhat to our own needs.  The methods are short
 * one-liners and therefore good candidates for inlining, so using this
 * class implies no overhead.
 */
#ifndef TADSWINDOW_H
#define TADSWINDOW_H

#include "common.h"

#include "frobcurses.h"

class FrobTadsWindow {
  private:
    // The curses window we maintain.
    WINDOW* fWin;

  public:
    /* Creates a new top-level window with 'lines' height, 'cols'
     * width, and coordinates 'yPos' and 'xPos'.
     */
    FrobTadsWindow( int lines, int cols, int yPos, int xPos )
    : fWin(newwin(lines, cols, yPos, xPos))
    { }

    ~FrobTadsWindow() { delwin(this->fWin); }

    /* Returns the height (lines) of the window.
     */
    int
    height() const
    { int y, x; getmaxyx(this->fWin, y, x); return y; }

    /* Returns the width (columns) of the window.
     */
    int
    width() const
    { int y, x; getmaxyx(this->fWin, y, x); return x; }

    /* Moves the cursor to line 'y', column 'x'.
     */
    int
    moveCursor( int y, int x ) { return wmove(this->fWin, y, x); }

    /* Gets a keystroke from the window.  A timeout can be set with
     * setTimeout() before calling this method.
     *
     * The returned value is the same as the curses getch() routine.
     */
    int
    getChar() { return wgetch(this->fWin); }

    /* Sets a timeout for subsequent input operations.  If 'timeout'
     * milliseconds pass and there's no input, ERR is returned.  If
     * 'timeout' is negative, the input methods will not use a timeout
     * but wait indefinitely for input.  A 0 'timeout' will only check
     * whether there's input already available in the input buffer and
     * fetch it, but will otherwise not block.
     */
    void
    setTimeout( int timeout ) { wtimeout(this->fWin, timeout); }

    /* Writes the string 'str' to the window at the specified
     * position.  Note the type of 'str'; it's not a 'char*'.  The
     * 'chtype' datatype is used by curses to store a string
     * together with the attributes of its characters.  This results
     * in faster output operations.
     */
    int
    printStr( int y, int x, chtype* str ) { return mvwaddchstr(this->fWin, y, x, str); }

    /* Writes the character 'ch' to the window at the specified
     * coordinates.
     */
    int
    printChar( int y, int x, const chtype ch ) { return mvwaddch(this->fWin, y, x, ch); }

    /* Returns the character at position (x,y).
     */
    chtype
    charAt( int y, int x ) { return mvwinch(this->fWin, y, x); }

    /* Blanks the window (erases its contents).
     */
    int
    blank() { return werase(this->fWin); }

    /* Enables/disables scrolling.
     */
    int
    enableScrolling( bool bf ) { return scrollok(this->fWin, bf); }

    /* Flushes the internal buffers so any pending output-operations
     * will be processed.  This is just a curses wrefresh().
     */
    int
    flush() { return wrefresh(this->fWin); }

    /* Mark the entire window as "touched"; throw away all
     * optimization information about which parts of the window have
     * changed, forcing curses to redraw all characters.
     */
    int
    touch() { return touchwin(this->fWin); }

    /* Enables/disables "keypad mode".  In this mode, input methods
     * (getChar(), etc.) will recognize special keys like function
     * keys, arrow keys, insert, delete, etc.  These are returned as
     * KEY_* values (as defined in <curses.h>).
     */
    int
    keypadMode( bool bf ) { return keypad(this->fWin, bf); }

    /* Enables/disables 8-bit input.  Normally, input is 7-bit,
     * which means that things like German unlauts und everything
     * else above the 7-bit ASCII range won't work.
     */
    int
    input8bit( bool bf ) { return meta(this->fWin, bf); }
};

#endif // TADSWINDOW_H
