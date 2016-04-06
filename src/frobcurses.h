#pragma once
/* This file includes the curses header file. It's either <curses.h> or
 * <ncurses.h> - whatever is available on the system.
 *
 * Everything in the included header is made extern "C". Normally, recent
 * curses versions already have extern "C" blocks in them. However, we provide
 * our own for older versions that might lack these blocks. It doesn't hurt
 * when such a block is specified twice.
 *
 * There's a potential problem when including a C++-unaware curses header; some
 * versions of curses define a 'bool' type. This can cause errors when
 * compiling in C++. Therefore, we undefine this symbol when in C++. This won't
 * work if 'bool' is typedef'ed though.
 *
 * Some curses versions define a clear() macro. This would cause some files in
 * tads2/ and tads3/ to fail to compile. Therefore, we #undef this macro is it
 * exists.
 */
#include "frob_config.h"

extern "C" {
    #if CURSES_HAVE_NCURSES_H
        #include <ncurses.h>
    #elif CURSES_HAVE_NCURSES_NCURSES_H
        #include <ncurses/ncurses.h>
    #elif CURSES_HAVE_NCURSES_CURSES_H
        #include <ncurses/curses.h>
    #elif CURSES_HAVE_CURSES_H
        #include <curses.h>
    #endif
}

// The TADS base code spits fire if 'clear' is defined as a macro.
#ifdef clear
#undef clear
#endif

// C++ has its own 'bool' type.
#ifdef bool
#undef bool
#endif
