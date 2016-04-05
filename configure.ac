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
AC_INIT([FrobTADS],[1.2.3],[realnc@gmail.com],[frobtads])
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
m4_include([m4/ax_require_defined.m4])
m4_include([m4/ax_append_flag.m4])
m4_include([m4/ax_check_compile_flag.m4])
m4_include([m4/ax_append_compile_flags.m4])
m4_include([m4/ax_pthread.m4])
m4_include([m4/ax_cxx_compile_stdcxx.m4])


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

# Require C++14 and C11
#
AX_CXX_COMPILE_STDCXX([14], [noext], [mandatory])
AC_LANG_PUSH(C)
AX_APPEND_COMPILE_FLAGS([-std=c11])
AC_LANG_POP(C)

# Disable strict-aliasing optimization since Tads does type punning on
# purpose in quite a few places.
AC_LANG_PUSH(C)
AX_APPEND_COMPILE_FLAGS([-fno-strict-aliasing])
AC_LANG_PUSH(C++)
AX_APPEND_COMPILE_FLAGS([-fno-strict-aliasing])
AC_LANG_POP(C++)
AC_LANG_POP(C)


#
# Checks for libraries.

# Check how to compile and link against the POSIX threads library.
#
AX_PTHREAD([], [AC_MSG_ERROR([POSIX threads seem to be missing; cannot continue])])
LIBS="$PTHREAD_LIBS $LIBS"
CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"

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

# Check for mkdir()/_mkdir() and wether mkdir() only takes one argument.
AX_FUNC_MKDIR

# The TADS base code considers the memicmp() function to be "standard".
# It actually isn't.  If the system doesn't provide it, our own
# implementation will be used.
#
AC_CHECK_FUNCS([memicmp putenv])

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

# Various other functions not available everywhere.
#
AC_CHECK_FUNCS([glob])

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

# The debbuger version of frob ("frobd") is not built by default.
#
AC_ARG_ENABLE(frobd, [  --enable-frobd          Build frobd, a version of frob usable by debuggers],
  [case "${enableval}" in
      yes) buildfrobd=true
           break ;;
      no)  buildfrobd=false
           break ;;
      *)   AC_MSG_ERROR(bad value ${enableval} for --enable-frobd)
           break ;;
  esac],
  [buildfrobd=false]
)
AM_CONDITIONAL([BUILD_FROBD], [test x$buildfrobd = xtrue])


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
