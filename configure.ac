#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# This file has been created by hand.  Autoconf automation tools might
# not be able to deal with it.
#
# The email address in the AC_INIT invocation is the address of the
# current maintainer, not the original author.  If the maintainer
# changes, replace the email address.  It is used to tell the user where
# to send bug reports.
#
# AC_PREREQ(VERSION) should contain the version of Autoconf as used by the
# maintainer.  Everyone else should update to at least this version of
# Autoconf.
#
AC_PREREQ(2.68)
AC_INIT([FrobTADS],[1.2.2],[realnc@gmail.com],[frobtads])
AC_CONFIG_SRCDIR(src/osfrobtads.h)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADERS(config.h)
AM_SILENT_RULES([yes])
AM_INIT_AUTOMAKE([-Wall foreign])

# The current TADS OEM version.  This must be set back to 0 each time
# FrobTADS is synced with a new version of the base code, and increased
# by 1 each time a new FrobTADS release is made that does not include a
# new version of the base code.  Note that this is a string, not a
# number.
#
AC_DEFINE([TADS_OEM_VERSION], ["1"], [Current TADS OEM version.])

# The current maintainer of FrobTADS.  Don't include an email address;
# the email should be specified in the AC_INIT invocation above.
#
AC_DEFINE([PACKAGE_MAINTAINER], ["Nikos Chantziaras"], [The person who currently maintains FrobTADS.])

# Include custom macros we ship ourselves.
#
m4_include([m4/ax_func_mkdir.m4])
m4_include([m4/ax_tls.m4])


#
# Checks for programs.

# Check for a C++ compiler.
#
AC_PROG_CXX

# Check for a C compiler.
#
AC_PROG_CC

# Check if the compiler accepts -c and -o at the same time.  We need
# this so that object files are placed in the same directory as the
# corresponding source file, rather than in the root directory.
#
AM_PROG_CC_C_O
AC_PROG_CXX_C_O

# On GNU C++, disable the strict-aliasing optimization since the T3VM
# crashes with it enabled.
# Disabled; the macro uses the options even on non-GCC compilers.  We
# don't want that.  Also, it would be better to be able to differenciate
# between GCC versions; GCC 4.2 and 4.3 don't seem to need this switch
# at all.
#AX_CXXFLAGS_GCC_OPTION(-fno-strict-aliasing)


#
# Checks for libraries.

# Try to find a curses library.  A curses library always provides the
# initscr() function, so we check for that.  We favor ncurses; if it
# isn't found we fall back to plain curses.  If that fails too, try
# pdcurses (normally pdcurses is just curses on most systems, but it
# doesn't hust to try pdcurses as a last resort.)
#
AC_SEARCH_LIBS([initscr], [ncurses curses pdcurses], [curseslibfound=true], [curseslibfound=false])

# Check for libcurl (network client library for http et al).
#
AC_SEARCH_LIBS([curl_global_init], [curl], [curllibfound=true], [curllibfound=false])


#
# Checks for header files.

AC_CHECK_HEADERS([limits.h stddef.h termios.h sys/ioctl.h])

# Prefer ncurses over curses.
#
AC_CHECK_HEADERS([ncurses.h curses.h], [break])

# Check for <sys/time.h>.  This is related to AC_HEADER_TIME below.
#
AC_CHECK_HEADERS([sys/time.h])

# Check if we may include both <time.h> and <sys/time.h>.  On some older
# systems, <sys/time.h> includes <time.h>, but <time.h> is not protected
# against multiple inclusion.  If it's safe to include both,
# TIME_WITH_SYS_TIME will be defined.
#
AC_HEADER_TIME

# Many systems lack <wchar.h>.
#
AC_CHECK_HEADERS([wchar.h], [wcharheaderfound=true], [wcharheaderfound=false])

# <glob.h> (pathname-search using patterns) is only available in POSIX.2.
#
AC_CHECK_HEADERS([glob.h])

# Locale support headers.
#
AC_CHECK_HEADERS([langinfo.h locale.h])

# If the use of TIOCGWINSZ requires <sys/ioctl.h>, then define
# GWINSZ_IN_SYS_IOCTL.  Otherwise TIOCGWINSZ can be found in <termios.h>.
#
AC_HEADER_TIOCGWINSZ


#
# Checks for typedefs, structures, and compiler characteristics.

# This normally checks if 'struct tm' is defined in <time.h>.  We could
# use this to include <sys/time.h> in case <time.h> lacks this struct.
# Unfortunately, the TADS base code includes <time.h> on its own in some
# places so it won't work.  This is plain paranoia anyway; I guess most
# systems define this struct in <time.h>.
#
#AC_STRUCT_TM

# Suggested by autoscan.
#
AC_C_CONST
AC_TYPE_SIZE_T
AC_C_VOLATILE

# Some systems don't provide the SIGWINCH signal (like MS Windows).
#
AC_CACHE_CHECK([for SIGWINCH signal support], ac_cv_sigwinch_signal,
[
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <signal.h>]], [[int foo = SIGWINCH;]])],
    [ac_cv_sigwinch_signal=yes],
    [ac_cv_sigwinch_signal=no]
  )
])
if test x$ac_cv_sigwinch_signal = xyes; then
  AC_DEFINE([HAVE_SIGWINCH], [1], [Define to 1 if you have the SIGWINCH signal.])
fi

# Some of the TADS base code tries to define "uchar", "ushort" and the
# like.  This will cause a compiler error if these types are already
# defined, so it checks for the OS_U*_DEFINED macros first.  We check
# for the existence of these types in both C and C++; the "common.h"
# header will then define the final OS_*_DEFINED macros according to
# whether it's being compiled by C or C++.
#
AC_LANG_PUSH(C)
AC_MSG_NOTICE([checking for presence of uchar, ushort, uint and ulong in C])
AC_CHECK_TYPE([uchar], [AC_DEFINE([C_UCHAR_DEFINED], [1], [Define to 1 if the uchar type exists in C.])])
AC_CHECK_TYPE([ushort], [AC_DEFINE([C_USHORT_DEFINED], [1], [Define to 1 if the ushort type exists in C.])])
AC_CHECK_TYPE([uint], [AC_DEFINE([C_UINT_DEFINED], [1], [Define to 1 if the uint type exists in C.])])
AC_CHECK_TYPE([ulong], [AC_DEFINE([C_ULONG_DEFINED], [1], [Define to 1 if the ulong type exists in C.])])
AC_LANG_POP(C)

# We can't use the same check twice, since autoconf would simply pull
# the results from the previous check from its cache.  In order to
# avoid that, we check each type with a space appended so it looks
# different; sounds stupid, but works :P
AC_LANG_PUSH(C++)
AC_MSG_NOTICE([checking for presence of uchar, ushort, uint and ulong in C++])
AC_CHECK_TYPE([uchar ], [AC_DEFINE([CXX_UCHAR_DEFINED], [1], [Define to 1 if the uchar type exists in C++.])])
AC_CHECK_TYPE([ushort ], [AC_DEFINE([CXX_USHORT_DEFINED], [1], [Define to 1 if the ushort type exists in C++.])])
AC_CHECK_TYPE([uint ], [AC_DEFINE([CXX_UINT_DEFINED], [1], [Define to 1 if the uint type exists in C++.])])
AC_CHECK_TYPE([ulong ], [AC_DEFINE([CXX_ULONG_DEFINED], [1], [Define to 1 if the ulong type exists in C++.])])
AC_LANG_POP(C++)

# Makefile.am needs to know if the system is big-endian (like Motorola
# and SPARC CPUs) or little-endian (like Intel and VAX).  We detect this
# here and let Automake know.  We'll report big-endian even if we can't
# actually detect the endianess; that's because the big-endian routines
# of TADS are actually generic and work for both big as well as
# little-endian CPUs.
#
AC_C_BIGENDIAN([cpuisbigendian=true], [cpuisbigendian=false], [cpuisbigendian=true])
AM_CONDITIONAL([CPU_IS_BIGENDIAN], [test x$cpuisbigendian = xtrue])

# Also define a CPU_IS_BIGENDIAN macro to 0 or 1.
if test x$cpuisbigendian = xtrue; then
  AC_DEFINE([CPU_IS_BIGENDIAN], [1], [Define to 1 if CPU is big endian.])
else
  AC_DEFINE([CPU_IS_BIGENDIAN], [0], [Define to 0 if CPU is little endian.])
fi

# Check the sizes of 'long', 'int' and 'short'.
#
AC_CHECK_SIZEOF([long])
AC_CHECK_SIZEOF([int])
AC_CHECK_SIZEOF([short])

# Check how the compiler handles TLS.
#
#AX_TLS([], [])
AX_TLS

# Some systems have the wchar functions (wcslen, wcscpy, etc) in the C
# library, but <wchar.h> doesn't declare them.
#
AC_CHECK_DECLS([wcslen, wcscpy], [], [], [#include <wchar.h>])

# Check if we can ioctl() TIOCGWINSZ.  This is the portable way of
# getting the terminal size.
#
AC_CACHE_CHECK([for TIOCGWINSZ ioctl support], ac_cv_tiocgwinsz_ioctl,
[
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
[[#ifndef GWINSZ_IN_SYS_IOCTL
#if HAVE_TERMIOS_H
#include <termios.h>
#endif
#endif
#if HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif]],
    [[struct winsize size; ioctl(0, TIOCGWINSZ, &size);]])],
    [ac_cv_tiocgwinsz_ioctl=yes],
    [ac_cv_tiocgwinsz_ioctl=no]
  )
])
if test x$ac_cv_tiocgwinsz_ioctl = xyes; then
  AC_DEFINE([HAVE_TIOCGWINSZ], [1], [Define to 1 if TIOCGWINSZ is available for ioctl.])
fi

# Check if we can ioctl() TIOCGSIZE.  This is the BSD4.3 way of getting
# the terminal size.
#
AC_CACHE_CHECK([for TIOCGSIZE ioctl support], ac_cv_tiocgsize_ioctl,
[
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
[[#if HAVE_TERMIOS_H
#include <termios.h>
#endif
#if HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif]],
    [[struct ttysize size; ioctl(0, TIOCGSIZE, &size);]])],
    [ac_cv_tiocgsize_ioctl=yes],
    [ac_cv_tiocgsize_ioctl=no]
  )
])
if test x$ac_cv_tiocgsize_ioctl = xyes; then
  AC_DEFINE([HAVE_TIOCGSIZE], [1], [Define to 1 if TIOCGSIZE is available for ioctl.])
fi


#
# Checks for library functions.

# Suggested by autoscan.
#
# Don't do the malloc() and realloc() checks because they result in a
# link error during a cross compile and we don't provide the needed
# fallback functions (rpl_malloc() and rpl_realloc()) anyway (that is,
# if those checks would fail while not cross-compiling, we would still
# get a link error.)
#AC_FUNC_MALLOC
#AC_FUNC_REALLOC
AC_FUNC_MEMCMP
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([memmove memset strchr putenv])

# Check for mkdir()/_mkdir() and wether mkdir() only takes one argument.
AX_FUNC_MKDIR

# The TADS base code considers the memicmp() function to be "standard".
# It actually isn't.  If the system doesn't provide it, our own
# implementation will be used.
#
AC_CHECK_FUNCS([memicmp])

# Tads uses the functions stricmp() and strnicmp() as if they were
# standard.  They aren't, but they have BSD 4.4 equivalents called
# strcasecmp() and strncasecmp().  Most Unices provide these.  If the
# system provides them, and the original ones aren't provided, map
# stricmp() to strcasecmp() and strnicmp() to strncasecmp().  If none of
# them exist, our code will provide default implementations.
#
AC_CHECK_FUNC(stricmp,
  [AC_DEFINE([HAVE_STRICMP], [1], [Define to 1 if you have stricmp.])],
  [
    AC_CHECK_FUNC(
      strcasecmp,
      [
        AC_DEFINE(
          [stricmp],
          [strcasecmp],
          [Map stricmp to strcasecmp if you lack the former.]
        )
        AC_DEFINE(
          [HAVE_STRCASECMP],
          [1],
          [Define to 1 if you have strcasecmp but not stricmp.]
        )
      ]
    )
  ]
)

AC_CHECK_FUNC(strnicmp,
  [AC_DEFINE([HAVE_STRNICMP], [1], [Define to 1 if have strnicmp.])],
  [
    AC_CHECK_FUNC(
      strncasecmp,
      [
        AC_DEFINE(
          [strnicmp],
          [strncasecmp],
          [Map strnicmp to strncasecmp if you lack the former.]
        )
        AC_DEFINE(
          [HAVE_STRNCASECMP],
          [1],
          [Define to 1 if you have strncasecmp but not strnicmp.]
        )
      ]
    )
  ]
)

# Try to find a function that changes the current working directory.  We
# first try chdir().  If not found, we search for SetCurrentDirectory().
#
# chdir() is listed in SVr4, SVID, POSIX, X/OPEN and 4.4BSD.
# SetCurrentDirectory() is MS-Windows.  Actually, chdir() should also be
# available in Windows, but I'm not sure.
#
AC_CHECK_FUNC(chdir,
  [AC_DEFINE([HAVE_CHDIR], [1], [Define to 1 if you have chdir.])],
  [AC_CHECK_FUNC(SetCurrentDirectory,
    [AC_DEFINE([HAVE_SETCURRENTDIRECTORY], [1], [Define to 1 if you have SetCurrentDirectory.])])]
)

# Tads wants a millisecond-precise timer.  The standard C library lacks
# a function that gets the current time with ms-precision.  Therefore,
# we search for one of 3 functions that can do that: clock_gettime(),
# gettimeofday() and ftime(), in that order.
#
# clock_gettime() has nanosecond-precision and is listed in SUSv2 and
# POSIX 1003.1-2001; gettimeofday() has microsecond-precision and is in
# SVr4, BSD 4.3 and POSIX 1003.1-2001; ftime() has millisecond-precision
# and is listed in the BSD 4.2 and POSIX 1003.1-2001 standards.
#
# clock_gettime() is the modern way of doing things, but is not widely
# available on older systems.  gettimeofday() should be available
# almost everywhere, while ftime() is an obsolete function but still
# does the job most of the time (some implementations are buggy and lack
# millisecond precision, like in early glibc2 < 2.1.1 libraries, in
# which case ftime() is no better than time(); no problem though since
# most such systems provide gettimeofday()).
#
# Note that on some systems we must link with the "rt" library to be
# able to use the clock_gettime() function.
#
# If clock_gettime() has been found, we also check if the system
# supports a monotonic clock (CLOCK_MONOTONIC).
#
AC_SEARCH_LIBS(clock_gettime, rt,
  [
    AC_DEFINE([HAVE_CLOCK_GETTIME], [1], [Define to 1 if you have clock_gettime.])
    AC_MSG_CHECKING([whether clock_gettime supports CLOCK_MONOTONIC])
    AC_COMPILE_IFELSE(
      [AC_LANG_PROGRAM(
        [[#include <time.h>]],
        [[clockid_t clockType = CLOCK_MONOTONIC;]]
      )],
      [
        AC_MSG_RESULT(yes)
        AC_DEFINE([HAVE_CLOCK_MONOTONIC], [1], [Define to 1 if clock_gettime supports CLOCK_MONOTONIC.])
      ],
      AC_MSG_RESULT(no)
    )
  ],
  [
    AC_CHECK_FUNCS([gettimeofday ftime], [break])
  ]
)

# Various other functions not available everywhere.
#
AC_CHECK_FUNCS([wcslen wcscpy glob])

# Check for use_default_colors(), which is an extension to curses and
# not available everywhere.
#
AC_CHECK_FUNCS([use_default_colors])

# Locale support functions.
#
AC_CACHE_CHECK([for nl_langinfo and CODESET], frob_cv_langinfo_codeset,
[
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <langinfo.h>]], [[char* cs = nl_langinfo(CODESET);]])],
    [frob_cv_langinfo_codeset=yes],
    [frob_cv_langinfo_codeset=no]
  )
])
if test x$frob_cv_langinfo_codeset = xyes; then
  AC_DEFINE([HAVE_LANGINFO_CODESET], [1], [Define to 1 if you have <langinfo.h> and nl_lan
ginfo(CODESET).])
fi

AC_CHECK_FUNCS([setlocale])


#
# Checks for C++ features.

# Switch to C++ before running the tests.
#
AC_LANG(C++)

# Check if the compiler supports the 'and', 'or' and 'not' keywords.
# Note that even some real compilers don't support them, although they
# are in the standard since 1997.  I'm not talking about VC++ 6; I said
# *real* compilers :*)
#
# If they aren't supported, we define them in common.h.  We don't define
# them here in order to avoid problems with some C (not C++) compilers
# that already define them as macros.
#
AC_CACHE_CHECK([whether the C++ compiler supports the and keyword], ac_cv_cxx_and_keyword,
[
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[int i;]], [[i = 0 and 1;]])],
    [ac_cv_cxx_and_keyword=yes],
    [ac_cv_cxx_and_keyword=no]
  )
])
if test x$ac_cv_cxx_and_keyword = xyes; then
  AC_DEFINE([HAVE_AND_KEYWORD], [1], [Define to 1 if you have the and keyword.])
fi

AC_CACHE_CHECK([whether the C++ compiler supports the or keyword], ac_cv_cxx_or_keyword,
[
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[int i;]], [[i = 0 or 1;]])],
    [ac_cv_cxx_or_keyword=yes],
    [ac_cv_cxx_or_keyword=no]
  )
])
if test x$ac_cv_cxx_or_keyword = xyes; then
  AC_DEFINE([HAVE_OR_KEYWORD], [1], [Define to 1 if you have the or keyword.])
fi

AC_CACHE_CHECK([whether the C++ compiler supports the not keyword], ac_cv_cxx_not_keyword,
[
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[int i;]], [[i = not 1;]])],
    [ac_cv_cxx_not_keyword=yes],
    [ac_cv_cxx_not_keyword=no]
  )
])
if test x$ac_cv_cxx_not_keyword = xyes; then
  AC_DEFINE([HAVE_NOT_KEYWORD], [1], [Define to 1 if you have the not keyword.])
fi

# Check if the compiler supports the 'bool' datatype.  Some older ones
# don't, so we'll have to typedef it in our common.h header.
#
AC_CHECK_TYPES(bool)

# Check if the compiler supports modern casting syntax
# (X_cast<type>(value)).  If yes, we'll define HAVE_X_CAST.  We use this
# to define the X_cast keywords as macros if the compiler lacks them.
#
# Original macros by Todd Veldhuizen and Luc Maisonobe <luc@spaceroots.org>.
# Updated by <realnc@gmail.com> and autoupdate.
#
AC_CACHE_CHECK(whether the compiler supports dynamic_cast<>, ac_cv_cxx_dynamic_cast,
[AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
    [[#include <typeinfo>
      class Base {public: Base(){} virtual void f() = 0;};
      class Derived: public Base {public: Derived(){} virtual void f(){}};
    ]], [[
      Derived d;
      Base& b=d;
      return dynamic_cast<Derived*>(&b) ? 0 : 1;
    ]])],
    [ac_cv_cxx_dynamic_cast=yes],
    [ac_cv_cxx_dynamic_cast=no]
  )
])
if test x$ac_cv_cxx_dynamic_cast = xyes; then
  AC_DEFINE(HAVE_DYNAMIC_CAST, 1, [define to 1 if the compiler supports dynamic_cast<>])
fi

AC_CACHE_CHECK(whether the compiler supports static_cast<>, ac_cv_cxx_static_cast,
[AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
    [[#include <typeinfo>
      class Base {public: Base(){} virtual void f() = 0;};
      class Derived: public Base {public: Derived(){} virtual void f(){}};
      int g(Derived&) {return 0;}
    ]], [[
      Derived d;
      Base& b = d;
      Derived& s = static_cast<Derived&> (b);
      return g(s);
    ]])],
    [ac_cv_cxx_static_cast=yes],
    [ac_cv_cxx_static_cast=no]
  )
])
if test x$ac_cv_cxx_static_cast = xyes; then
  AC_DEFINE(HAVE_STATIC_CAST, 1, [define to 1 if the compiler supports static_cast<>])
fi

AC_CACHE_CHECK(whether the compiler supports reinterpret_cast<>, ac_cv_cxx_reinterpret_cast,
[AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM(
    [[#include <typeinfo>
      class Base {public: Base(){} virtual void f() = 0;};
      class Derived: public Base {public: Derived(){} virtual void f(){}};
      class Unrelated {public: Unrelated(){}};
      int g(Unrelated&) {return 0;}
    ]], [[
      Derived d;
      Base& b=d;
      Unrelated& e=reinterpret_cast<Unrelated&>(b);
      return g(e);
    ]])],
    [ac_cv_cxx_reinterpret_cast=yes],
    [ac_cv_cxx_reinterpret_cast=no]
  )
])
if test x$ac_cv_cxx_reinterpret_cast = xyes; then
  AC_DEFINE(HAVE_REINTERPRET_CAST, 1, [define to 1 if the compiler supports reinterpret_cast<>])
fi


#
# Additional 'configure' command-line options.

# Add '--enable-t3debug', which builds the debug-version of TADS 3 and
# also enables the TADS 3 test suite.
#
AC_ARG_ENABLE(t3debug, [  --enable-t3debug        Build the debug version of TADS 3],
  [case "${enableval}" in
      yes) t3debugbuild=true
           break ;;
      no)  t3debugbuild=false
           break ;;
      *)   AC_MSG_ERROR(bad value ${enableval} for --enable-t3debug)
           break ;;
  esac],
  [t3debugbuild=false]
)
AM_CONDITIONAL(T3_DEBUG_BUILD, test x$t3debugbuild = xtrue)

# Add '--enable-static-link', which allows the user to build static
# binaries on systems that default to dynamic linking.
#
AC_MSG_CHECKING(whether to create static or dynamic binaries)
AC_ARG_ENABLE(static-link, [  --enable-static-link    Create statically-linked binaries],
  [case "${enableval}" in
      yes) LDFLAGS="$LDFLAGS -static"
           AC_MSG_RESULT(statically linked)
           break ;;
      no)  AC_MSG_RESULT(let the system decide)
           break ;;
      *)   AC_MSG_ERROR(bad value ${enableval} for --enable-static-link)
           break ;;
  esac],
  AC_MSG_RESULT(let the system decide)
)

# Add '--enable-error-checking', which allows the user to enable some
# extra error-checking code in the TADS2 VM (stack overflows and such).
#
AC_ARG_ENABLE(error-checking, [  --enable-error-checking Enable TADS2 runtime error-checks],
  [case "${enableval}" in
      yes) t2runtimechecks=true
           break ;;
      no)  t2runtimechecks=false
           break ;;
      *)   AC_MSG_ERROR(bad value ${enableval} for --enable-error-checking)
           break ;;
  esac],
  [t2runtimechecks=false]
)
AM_CONDITIONAL([T2_RUNTIME_CHECKING], [test x$t2runtimechecks = xtrue])


# Tell Automake to build the interpreter only if a curses library was found.
#
AM_CONDITIONAL([BUILD_INTERPRETER], [test x$curseslibfound = xtrue])

# Build the compilers only if the user didn't disable them.
#
AC_ARG_ENABLE(t2-compiler, [  --disable-t2-compiler   Do not build the TADS 2 compiler],
  [case "${enableval}" in
      yes) buildt2compiler=true
           break ;;
      no)  buildt2compiler=false
           break ;;
      *)   AC_MSG_ERROR(bad value ${enableval} for --enable-t2-compiler)
           break ;;
  esac],
  [buildt2compiler=true]
)
AC_ARG_ENABLE(t3-compiler, [  --disable-t3-compiler   Do not build the TADS 3 compiler],
  [case "${enableval}" in
      yes) buildt3compiler=true
           break ;;
      no)  buildt3compiler=false
           break ;;
      *)   AC_MSG_ERROR(bad value ${enableval} for --enable-t3-compiler)
           break ;;
  esac],
  [buildt3compiler=true]
)
AM_CONDITIONAL([BUILD_T2_COMPILER], [test x$buildt2compiler = xtrue])
AM_CONDITIONAL([BUILD_T3_COMPILER], [test x$buildt3compiler = xtrue])

# If the system lacks the <wchar.h> header, let Automake know.
#
AM_CONDITIONAL([WCHAR_HEADER_MISSING], [test x$wcharheaderfound = xfalse])

# Tell Automake when the compiler doesn't supports TLS.
#
AM_CONDITIONAL([NO_TLS], [test x$ac_cv_tls = xnone])


#
# Generate the output files.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

if test x$curseslibfound = xfalse; then
  AC_MSG_NOTICE([***])
  AC_MSG_NOTICE([*** A curses library is missing.  The interpreter cannot be built.])
fi

if test x$curllibfound = xfalse; then
  AC_MSG_NOTICE([***])
  AC_MSG_NOTICE([*** libcurl is missing.  The interpreter cannot be built.])
fi
