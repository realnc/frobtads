/* FrobTadsApplicationCurses class implementation.
 */
#include "common.h"

#include <string.h>
#if HAVE_TIOCGWINSZ || HAVE_TIOCGSIZE
# if (!defined(GWINSZ_IN_SYS_IOCTL) || HAVE_TIOCGSIZE) && HAVE_TERMIOS_H
#  include <termios.h>
# endif
# if HAVE_SYS_IOCTL_H
#  include <sys/ioctl.h>
# endif
#endif

#include "frobtadsappcurses.h"
#include "colors.h"


/* Service routine.  Gets the terminal's size.
 * On success, stores the terminal dimensions in 'width' and
 * 'height' and returns true.  On failure, returns false and
 * 'width' nor 'height' are modified.
 */
static bool
getTermSize( unsigned& width, unsigned& height )
{
	// If the system has support for the TIOCGWINSZ or TIOCGSIZE
	// ioctl, use it to obtain the terminal's dimensions.
	int res = -1;
	unsigned w = 0;
	unsigned h = 0;
#if HAVE_TIOCGWINSZ
	winsize size;
	size.ws_col = size.ws_row = 0;
	res = ioctl(0, TIOCGWINSZ, &size);
	w = size.ws_col;
	h = size.ws_row;
#elif HAVE_TIOCGSIZE
	ttysize size;
	size.ts_cols = size.ts_lines = 0;
	res = ioctl(0, TIOCGSIZE, &size);
	w = size.ts_cols;
	h = size.ts_lines;
#endif
	if (res < 0 or w == 0 or h == 0) {
		// The call failed or the system doesn't seem to know
		// the terminal's size.
		return false;
	}
	width = w;
	height = h;
	return true;
}


FrobTadsApplicationCurses::FrobTadsApplicationCurses( const FrobOptions& opts )
: FrobTadsApplication(opts), fGameWindow(0), fDispBuf(0)
{
	// Before starting curses, we set the LINES and COLUMNS env.
	// variables to a "maximum" value.  This is needed in case we're
	// running on a system with an old/broken curses implementation
	// (Solaris curses, for example).  This forces curses to
	// allocate enough internal memory for the windows in case the
	// terminal gets resized.  Note that we only do this if
	// detection of the terminal size is possible at all.
#if HAVE_PUTENV
	unsigned bogusX, bogusY; // Dummies.
	if (getTermSize(bogusX, bogusY)) {
		// The strings become part of the environment; they have to be
		// static.
		static char lines[] = "LINES=225";
		static char columns[] = "COLUMNS=300";
		putenv(lines);
		putenv(columns);
	}
#endif

	// Fire-up curses.
	initscr();

	// Make characters typed by the user immediately available to
	// the program.
	cbreak();

	// Tell courses not to echo user-input (TADS does the echoing).
	noecho();

	// Hide the cursor.  This avoids the "crazy cursor jumping"
	// problem on *really* slow terminals, as TADS constantly moves
	// the cursor all over the place.
	curs_set(0);

	// Tell courses not to try to translate the RETURN/ENTER key on
	// input (or else getch() and friends would never report that
	// key), and the '\n' character on output (since the osgen layer
	// handles newlines).
	nonl();

	// Initialize colors if we want color support and the terminal supports it.
	if ((opts.useColors and has_colors()) or opts.forceColors) {
		this->fColorsEnabled = true;
		start_color();
		// Assign terminal default foreground/background colors
		// to color number -1 / color pair 0.
#ifdef HAVE_USE_DEFAULT_COLORS
		if (opts.defColors) {
			use_default_colors();
		}
#endif
		// Initialize the curses color-pairs we use.
		//
		// This is paranoia, but better to make sure that curses
		// will use the right colors.  The paranoid part of this
		// is that we assume that there might be some curses
		// version out there that does not use monotonically
		// increasing values for the COLOR_* symbols.
		init_pair(makeColorPair(FROB_BLACK,   FROB_BLACK), COLOR_BLACK,   COLOR_BLACK);
		init_pair(makeColorPair(FROB_RED,     FROB_BLACK), COLOR_RED,     COLOR_BLACK);
		init_pair(makeColorPair(FROB_GREEN,   FROB_BLACK), COLOR_GREEN,   COLOR_BLACK);
		init_pair(makeColorPair(FROB_YELLOW,  FROB_BLACK), COLOR_YELLOW,  COLOR_BLACK);
		init_pair(makeColorPair(FROB_BLUE,    FROB_BLACK), COLOR_BLUE,    COLOR_BLACK);
		init_pair(makeColorPair(FROB_MAGENTA, FROB_BLACK), COLOR_MAGENTA, COLOR_BLACK);
		init_pair(makeColorPair(FROB_CYAN,    FROB_BLACK), COLOR_CYAN,    COLOR_BLACK);
		// White on black is wired to color pair 0 by
		// definition; we can't and don't need to initialize it.
		//init_pair(makeColorPair(FROB_WHITE,   FROB_BLACK), COLOR_WHITE,   COLOR_BLACK);

		init_pair(makeColorPair(FROB_BLACK,   FROB_RED), COLOR_BLACK,   COLOR_RED);
		init_pair(makeColorPair(FROB_RED,     FROB_RED), COLOR_RED,     COLOR_RED);
		init_pair(makeColorPair(FROB_GREEN,   FROB_RED), COLOR_GREEN,   COLOR_RED);
		init_pair(makeColorPair(FROB_YELLOW,  FROB_RED), COLOR_YELLOW,  COLOR_RED);
		init_pair(makeColorPair(FROB_BLUE,    FROB_RED), COLOR_BLUE,    COLOR_RED);
		init_pair(makeColorPair(FROB_MAGENTA, FROB_RED), COLOR_MAGENTA, COLOR_RED);
		init_pair(makeColorPair(FROB_CYAN,    FROB_RED), COLOR_CYAN,    COLOR_RED);
		init_pair(makeColorPair(FROB_WHITE,   FROB_RED), COLOR_WHITE,   COLOR_RED);

		init_pair(makeColorPair(FROB_BLACK,   FROB_GREEN), COLOR_BLACK,   COLOR_GREEN);
		init_pair(makeColorPair(FROB_RED,     FROB_GREEN), COLOR_RED,     COLOR_GREEN);
		init_pair(makeColorPair(FROB_GREEN,   FROB_GREEN), COLOR_GREEN,   COLOR_GREEN);
		init_pair(makeColorPair(FROB_YELLOW,  FROB_GREEN), COLOR_YELLOW,  COLOR_GREEN);
		init_pair(makeColorPair(FROB_BLUE,    FROB_GREEN), COLOR_BLUE,    COLOR_GREEN);
		init_pair(makeColorPair(FROB_MAGENTA, FROB_GREEN), COLOR_MAGENTA, COLOR_GREEN);
		init_pair(makeColorPair(FROB_CYAN,    FROB_GREEN), COLOR_CYAN,    COLOR_GREEN);
		init_pair(makeColorPair(FROB_WHITE,   FROB_GREEN), COLOR_WHITE,   COLOR_GREEN);

		init_pair(makeColorPair(FROB_BLACK,   FROB_YELLOW), COLOR_BLACK,   COLOR_YELLOW);
		init_pair(makeColorPair(FROB_RED,     FROB_YELLOW), COLOR_RED,     COLOR_YELLOW);
		init_pair(makeColorPair(FROB_GREEN,   FROB_YELLOW), COLOR_GREEN,   COLOR_YELLOW);
		init_pair(makeColorPair(FROB_YELLOW,  FROB_YELLOW), COLOR_YELLOW,  COLOR_YELLOW);
		init_pair(makeColorPair(FROB_BLUE,    FROB_YELLOW), COLOR_BLUE,    COLOR_YELLOW);
		init_pair(makeColorPair(FROB_MAGENTA, FROB_YELLOW), COLOR_MAGENTA, COLOR_YELLOW);
		init_pair(makeColorPair(FROB_CYAN,    FROB_YELLOW), COLOR_CYAN,    COLOR_YELLOW);
		init_pair(makeColorPair(FROB_WHITE,   FROB_YELLOW), COLOR_WHITE,   COLOR_YELLOW);

		init_pair(makeColorPair(FROB_BLACK,   FROB_BLUE), COLOR_BLACK,   COLOR_BLUE);
		init_pair(makeColorPair(FROB_RED,     FROB_BLUE), COLOR_RED,     COLOR_BLUE);
		init_pair(makeColorPair(FROB_GREEN,   FROB_BLUE), COLOR_GREEN,   COLOR_BLUE);
		init_pair(makeColorPair(FROB_YELLOW,  FROB_BLUE), COLOR_YELLOW,  COLOR_BLUE);
		init_pair(makeColorPair(FROB_BLUE,    FROB_BLUE), COLOR_BLUE,    COLOR_BLUE);
		init_pair(makeColorPair(FROB_MAGENTA, FROB_BLUE), COLOR_MAGENTA, COLOR_BLUE);
		init_pair(makeColorPair(FROB_CYAN,    FROB_BLUE), COLOR_CYAN,    COLOR_BLUE);
		init_pair(makeColorPair(FROB_WHITE,   FROB_BLUE), COLOR_WHITE,   COLOR_BLUE);

		init_pair(makeColorPair(FROB_BLACK,   FROB_MAGENTA), COLOR_BLACK,   COLOR_MAGENTA);
		init_pair(makeColorPair(FROB_RED,     FROB_MAGENTA), COLOR_RED,     COLOR_MAGENTA);
		init_pair(makeColorPair(FROB_GREEN,   FROB_MAGENTA), COLOR_GREEN,   COLOR_MAGENTA);
		init_pair(makeColorPair(FROB_YELLOW,  FROB_MAGENTA), COLOR_YELLOW,  COLOR_MAGENTA);
		init_pair(makeColorPair(FROB_BLUE,    FROB_MAGENTA), COLOR_BLUE,    COLOR_MAGENTA);
		init_pair(makeColorPair(FROB_MAGENTA, FROB_MAGENTA), COLOR_MAGENTA, COLOR_MAGENTA);
		init_pair(makeColorPair(FROB_CYAN,    FROB_MAGENTA), COLOR_CYAN,    COLOR_MAGENTA);
		init_pair(makeColorPair(FROB_WHITE,   FROB_MAGENTA), COLOR_WHITE,   COLOR_MAGENTA);

		init_pair(makeColorPair(FROB_BLACK,   FROB_CYAN), COLOR_BLACK,   COLOR_CYAN);
		init_pair(makeColorPair(FROB_RED,     FROB_CYAN), COLOR_RED,     COLOR_CYAN);
		init_pair(makeColorPair(FROB_GREEN,   FROB_CYAN), COLOR_GREEN,   COLOR_CYAN);
		init_pair(makeColorPair(FROB_YELLOW,  FROB_CYAN), COLOR_YELLOW,  COLOR_CYAN);
		init_pair(makeColorPair(FROB_BLUE,    FROB_CYAN), COLOR_BLUE,    COLOR_CYAN);
		init_pair(makeColorPair(FROB_MAGENTA, FROB_CYAN), COLOR_MAGENTA, COLOR_CYAN);
		init_pair(makeColorPair(FROB_CYAN,    FROB_CYAN), COLOR_CYAN,    COLOR_CYAN);
		init_pair(makeColorPair(FROB_WHITE,   FROB_CYAN), COLOR_WHITE,   COLOR_CYAN);

		init_pair(makeColorPair(FROB_BLACK,   FROB_WHITE), COLOR_BLACK,   COLOR_WHITE);
		init_pair(makeColorPair(FROB_RED,     FROB_WHITE), COLOR_RED,     COLOR_WHITE);
		init_pair(makeColorPair(FROB_GREEN,   FROB_WHITE), COLOR_GREEN,   COLOR_WHITE);
		init_pair(makeColorPair(FROB_YELLOW,  FROB_WHITE), COLOR_YELLOW,  COLOR_WHITE);
		init_pair(makeColorPair(FROB_BLUE,    FROB_WHITE), COLOR_BLUE,    COLOR_WHITE);
		init_pair(makeColorPair(FROB_MAGENTA, FROB_WHITE), COLOR_MAGENTA, COLOR_WHITE);
		init_pair(makeColorPair(FROB_CYAN,    FROB_WHITE), COLOR_CYAN,    COLOR_WHITE);
		init_pair(makeColorPair(FROB_WHITE,   FROB_WHITE), COLOR_WHITE,   COLOR_WHITE);
	}
}


FrobTadsApplicationCurses::~FrobTadsApplicationCurses()
{
	// Enable the cursor.
	curs_set(0);

	// Flush any pending output so we won't lose any "thanks for
	// playing" messages when "pausing prior to exit" is disabled.
	if (not this->options.exitPause) this->fGameWindow->flush();

	// Delete our windows, if we have any.
	if (this->fGameWindow != 0) delete this->fGameWindow;

	// Shut down curses.
	endwin();

	// Delete our optimization buffer.
	delete[] this->fDispBuf;
}


void
FrobTadsApplicationCurses::init()
{
	// Delete the current window, if we have one.
	if (this->fGameWindow != 0) delete this->fGameWindow;

	// Create the main window.  Make it fill the whole terminal.  We
	// initialize the dimensions to 0 in case getTermSize() is
	// unable to detect the actual size, since in curses/ncurses 0
	// for width and height actually means "fullscreen".
	unsigned width = 0;
	unsigned height = 0;
	bool sizeDetected = getTermSize(width, height);
	if (sizeDetected) {
		// Update the environment in case we're running inside
		// an old/broken terminal or are using an old/broken
		// version of curses.
#if HAVE_PUTENV
		// Note that the strings have to be static, since
		// putenv() does not always copy the string arguments
		// but points the environment to the application's
		// address space (and therefore the application would
		// segfault if the strings were not static).
		static char linesEnv[32];
		static char columnsEnv[32];
		char tmp[16];
		strcpy(linesEnv, "LINES=");
		strcpy(columnsEnv, "COLUMNS=");
		sprintf(tmp, "%u", height);
		strcat(linesEnv, tmp);
		sprintf(tmp, "%u", width);
		strcat(columnsEnv, tmp);
		// Normally, since the strings are part of the
		// environment, we would need to use putenv() only once;
		// changing the strings later would also change the
		// environment, since the strings are part of it.
		// However, not every system follows the standard, so we
		// need to putenv() the strings over and over again.
		putenv(linesEnv);
		putenv(columnsEnv);
		// Enforce the detected size upon curses.
		// TODO: Is this needed?  Or even allowed?  For now, we
		// assume it's neither.
		//COLS = width;
		//LINES = height;
#endif
	}

	// Reset curses, in case the terminal's size has changed.
	endwin();
	refresh();
	// Fixes problems with old curses versions.
	wclear(stdscr);

	// Create the window.
	this->fGameWindow = new FrobTadsWindow(height, width, 0, 0);

	// Disable scrolling.
	this->fGameWindow->enableScrolling(false);

	// Tell the window to recognize keypad keys (function keys,
	// cursor-keys, etc.) during input.
	this->fGameWindow->keypadMode(true);

	// Enable 8-bit characters during input.
	this->fGameWindow->input8bit(true);

	// Create the buffer that we use to optimize output.
	if (this->fDispBuf != 0) delete[] this->fDispBuf;
	this->fDispBuf = new chtype[this->fGameWindow->width() + 1];

	// Explicitly mark the window as touched, to work around a
	// curses color and screen corruption issue.
	this->fGameWindow->touch();

	// Fix for curses color corruption problem.  The window is
	// cleared to a color (other than black and white) which contrasts
	// with the normal background color.
	if (sizeDetected) {
		int fillcolor;
		if (this->options.bgColor == FROB_BLUE) {
			fillcolor = ossgetcolor(OSGEN_COLOR_WHITE, OSGEN_COLOR_RED, 0, 0);
		} else {
			fillcolor = ossgetcolor(OSGEN_COLOR_WHITE, OSGEN_COLOR_BLUE, 0, 0);
		}
		ossclr(0, 0, height - 1, width - 1, fillcolor);
		this->fGameWindow->flush();
	}
}


void
FrobTadsApplicationCurses::clear( int top, int left, int bottom, int right, int attrs )
{
	const chtype c = attrs | ' ';
	for (int y = top; y <= bottom; ++y) {
		for (int x = left; x <= right; ++x) {
			this->fGameWindow->printChar(y, x, c);
		}
	}
}


void
FrobTadsApplicationCurses::scrollRegionUp( int top, int left, int bottom, int right, int attrs )
{
	// Old curses versions might break when using overlapping
	// windows, so we do the scrolling by hand.
	for (int y = bottom; y > top; --y) {
		for (int x = left; x <= right; ++x) {
			const chtype c = this->fGameWindow->charAt(y-1, x);
			this->fGameWindow->printChar(y, x, c);
		}
	}

	// Clear the last line.
	const chtype c = attrs | ' ';
	for (int i = left; i <= right; ++i) this->fGameWindow->printChar(top, i, c);

	// If soft-scrolling is enabled, update the display so that we
	// have a visible scrolling-effect.
	if (this->options.softScroll) this->fGameWindow->flush();
}


void
FrobTadsApplicationCurses::scrollRegionDown( int top, int left, int bottom, int right, int attrs )
{
	// This works like scrollRegionUp(), just the other way around.
	for (int y = top; y < bottom; ++y) {
		for (int x = left; x <= right; ++x) {
			this->fGameWindow->printChar(y, x, this->fGameWindow->charAt(y+1, x));
		}
	}
	const chtype c = attrs | ' ';
	for (int i = left; i <= right; ++i) this->fGameWindow->printChar(bottom, i, c);
	if (this->options.softScroll) this->fGameWindow->flush();
}


int
FrobTadsApplicationCurses::getRawChar( bool cursorVisible, int timeout )
{
	// The character we read.
	int c;

	// Tell osgen to check if it should redraw the screen.
	osssb_redraw_if_needed();

	// Some curses versions don't automatically update the display
	// prior to input, so we must explicitly request it.
	this->fGameWindow->flush();

	// Prepare for timed input.
	this->fGameWindow->setTimeout(timeout);
	this->fRemainingTimeout = timeout;

	// Enable the cursor, if needed.  We won't enable it if the
	// timeout is very small, even if the caller requested it.
	// "Small" means under 1 second, which is what most terminals
	// use for a cursor blink.
	if (cursorVisible and (timeout > 1000 or timeout < 1)) curs_set(1);

	// On some systems, ncurses is configured to supply its own
	// SIGWINCH handler.  If this is the case, getch() returns
	// KEY_RESIZE when the terminal has been resized.  We can't rely
	// on this behavior to handle screen resizes, but we must still
	// recognize this return code; here we simply ignore it.  Note
	// that curses doesn't know anything about KEY_RESIZE; it's an
	// ncurses thing.  We only check for it if it is defined.
#ifdef KEY_RESIZE
	while ((c = this->fGameWindow->getChar()) == KEY_RESIZE)
		;
#else
	c = this->fGameWindow->getChar();
#endif
	this->fRemainingTimeout = 0;

	// Some terminals don't return KEY_BACKSPACE for the
	// backspace-key, but some opaque platform-specific code.  If
	// this is the case, map it to KEY_BACKSPACE.
	if (c == erasechar()) c = KEY_BACKSPACE;

	// Similarly for KEY_DL.
	if (c == killchar()) c = KEY_DL;

	curs_set(0);
	return c;
}
