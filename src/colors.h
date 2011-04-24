/* Color definitions for FrobTADS.
 */
#ifndef COLORS_H
#define COLORS_H

#include "common.h"

/* Takes two FROB_* colors (foreground, background), and returns a
 * color-pair.  The result is always between 0 and 63 and suitable as an
 * argument to the curses COLOR_PAIR() macro.  Example:
 *
 *   COLOR_PAIR(makeColorPair(FROB_GREEN, FROB_BLACK));
 */
inline int
makeColorPair(int fg, int bg)
{
    int res = fg + (bg << 3);
    // Colorpair 0 is wired to white on black and cannot be changed.
    // Therefore, we cannot use 0 for black on black; we'll exchange
    // it with pair 7, which is white on black.
    if (res == 0) return 7;
    if (res == 7) return 0;
    return res;
}

#define FROB_BLACK   0
#define FROB_RED     1
#define FROB_GREEN   2
#define FROB_YELLOW  3
#define FROB_BLUE    4
#define FROB_MAGENTA 5
#define FROB_CYAN    6
#define FROB_WHITE   7

#endif // COLORS_H
