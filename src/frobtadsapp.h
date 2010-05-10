/* The FrobTadsApplication class represents the main application object.
 * It provides an easy to use interface to the screen.   It is also
 * responsible for starting the TADS2/3 VM.
 *
 * This class defines an abstract screen I/O interface which has to be
 * implemented in an API-specific subclass.
 */
#ifndef FROBTADSAPP_H
#define FROBTADSAPP_H

#include "common.h"

#include "os.h"
extern "C" {
#include "osgen.h"
}


/* Global application pointer.
 */
extern class FrobTadsApplication* globalApp;

class FrobTadsApplication {
  public:
	// Signal handlers are such a pain in the ass...  Has to be
	// friend.
	friend RETSIGTYPE winResizeHandler(int);

	// Instead of using a gazillion of constructor-arguments, we
	// group them inside this structure.
	struct FrobOptions {
		bool useColors;         // Enable colors?
		bool forceColors;       // Force colors?
		bool defColors;         // Use default colors for color pair 0?
		bool softScroll;        // Scroll softly?
		bool exitPause;         // Pause prior to exit?
		bool changeDir;         // Change to the game's directory?
		int textColor;          // Default text color.
		int bgColor;            // Default background color.
		int statTextColor;      // Default text color of statusline.
		int statBgColor;        // Default background color of statusline.
		unsigned scrollBufSize; // Scroll-back buffer size.
		int safetyLevel;        // File I/O safety level.
		char characterSet[40];  // Local character set name.
	};

	// Our options.  Once set, they remain constant during run-time
	// so there's no reason to protect them.
	const FrobOptions options;

  private:
	/* Runs the T2VM.
	 */
	int
	fRunTads2( char* filename );

	/* Runs the T3VM.
	 */
	int
	fRunTads3( char* filename );

  protected:
	// We store the remaining input-timeout here in case a
	// window-resize occurs; the signal handler can then restore it.
	int fRemainingTimeout;

	// Are we using colors?
	bool fColorsEnabled;

  public:
	FrobTadsApplication( const FrobOptions& opts );

	/* Are colors enabled?
	 */
	bool
	colorsEnabled() const
	{ return this->fColorsEnabled; }

	/* Starts the VM and returns its exit code.  Which VM is started
	 * is given by the 'vm' argument: 0 for TADS2, non-0 for TADS3.
	 */
	int
	runTads( const char* filename, int vm );

	/* Change the current working directory, if possible.  Returns
	 * true on success, false on failure.
	 */
	bool
	changeDirectory( const char* dir );


	/* ============================================================
	 * Pure virtual functions follow.  This is the interface that
	 * has to be implemented in an API-specific subclass.
	 * ============================================================
	 */
  protected:
	/* Initialize the display.  This is called once just prior to
	 * starting a game.
	 */
	virtual void
	init() = 0;

	/* Re-initialize the display and any internal book-keeping data
	 * if needed.  This is called every time the terminal
	 * dimensions change.  This usually means the new dimensions
	 * have to be detected and a new output window will be created.
	 */
	virtual void
	resizeEvent() = 0;

  public:
	/* Moves the cursor to the specified position.
	 */
	virtual void
	moveCursor( int line, int column ) = 0;

	/* Prints a string at the specified position, using 'attrs' as
	 * atributes.
	 */
	virtual void
	print( int line, int column, int attrs, const char* str ) = 0;

	/* Flush any pending output.  That means everything that is not
	 * printed yet due to buffered output (if applicable) should be
	 * printed now.
	 */
	virtual void
	flush() = 0;

	/* Clears a portion of the screen.
	 */
	virtual void
	clear( int top, int left, int bottom, int right, int attrs ) = 0;

	/* Scroll the specified region of the screen 1 line up/down.
	 * 'attrs' holds the attributes of the generated empty lines.
	 */
	virtual void
	scrollRegionUp( int top, int left, int bottom, int right, int attrs ) = 0;

	virtual void
	scrollRegionDown( int top, int left, int bottom, int right, int attrs ) = 0;

	/* Reads a character from the keyboard and returns it.  If a
	 * function key has been pressed, then a curses KEY_* value is
	 * returned (have a look at your system's <curses.h> header for
	 * a list of KEY_ macros).
	 *
	 * If 'cursorVisible' is false, no cursor is shown while waiting
	 * for a key-press.
	 *
	 * If 'timeout' is > -1, then the routine will wait for a key
	 * only for 'timeout' milliseconds.  If a key has been pressed
	 * before the timeout expires it is returned as usual.  If the
	 * timeout expires and there's still no key, ERR is returned.
	 * A 0 timeout should not block, but still check if there's
	 * input waiting in an input buffer (if applicable.)
	 *
	 * Note that KEY_BACKSPACE and KEY_DL are handled correctly; you
	 * don't need to use the low-level curses routines erasechar()
	 * and killchar().  Just check for KEY_BACKSPACE and KEY_DL.
	 */
	virtual int
	getRawChar( bool cursorVisible, int timeout ) = 0;

	/* Pause for 'ms' milliseconds.
	 */
	virtual void
	sleep( int ms ) = 0;

	/* Returns the height (lines) of the screen.
	 */
	virtual int
	height() const = 0;

	/* Returns the width (columns) of the screen.
	 */
	virtual int
	width() const = 0;
};

#endif // FROBTADSAPP
