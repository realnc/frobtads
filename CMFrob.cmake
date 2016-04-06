find_package(Threads REQUIRED)
find_package(Curses REQUIRED)
find_package(CURL REQUIRED)

set(CMAKE_REQUIRED_LIBRARIES ${CURSES_LIBRARIES})
check_function_exists(use_default_colors HAVE_USE_DEFAULT_COLORS)
set(CMAKE_REQUIRED_LIBRARIES)
check_include_files(termios.h HAVE_TERMIOS_H)
check_include_files(sys/ioctl.h HAVE_SYS_IOCTL_H)

# Some systems might now provide the SIGWINCH signal.
check_symbol_exists(SIGWINCH signal.h HAVE_SIGWINCH)

# If the use of TIOCGWINSZ requires <sys/ioctl.h>, then define
# GWINSZ_IN_SYS_IOCTL. Otherwise we assume that TIOCGWINSZ can be found in
# <termios.h>.
check_symbol_exists(TIOCGWINSZ sys/ioctl.h GWINSZ_IN_SYS_IOCTL)

# Check if we can ioctl() TIOCGWINSZ. This is the portable way of getting the
# terminal size.
if (GWINSZ_IN_SYS_IOCTL)
    set(CMAKE_REQUIRED_DEFINITIONS -DGWINSZ_IN_SYS_IOCTL)
endif()
if (HAVE_TERMIOS_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_TERMIOS_H)
endif()
if (HAVE_SYS_IOCTL_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_SYS_IOCTL_H)
endif()
check_cxx_source_compiles ("
    #ifndef GWINSZ_IN_SYS_IOCTL
        #if HAVE_TERMIOS_H
            #include <termios.h>
        #endif
    #endif
    #if HAVE_SYS_IOCTL_H
        #include <sys/ioctl.h>
    #endif
    int main() {struct winsize size; ioctl(0, TIOCGWINSZ, &size);}"
    HAVE_TIOCGWINSZ
)

# Check if we can ioctl() TIOCGSIZE. This is the BSD way of getting the
# terminal size.
check_cxx_source_compiles ("
    #if HAVE_TERMIOS_H
        #include <termios.h>
    #endif
    #if HAVE_SYS_IOCTL_H
        #include <sys/ioctl.h>
    #endif
    int main() {struct ttysize size; ioctl(0, TIOCGSIZE, &size);}"
    HAVE_TIOCGSIZE
)
set(CMAKE_REQUIRED_DEFINITIONS)

# Sources needed only by the interpreter.
set (
    FROBSOURCES
    src/colors.h
    src/frobappctx.cc
    src/frobappctx.h
    src/frobtadsapp.cc
    src/frobtadsapp.h
    src/frobtadsappcurses.cc
    src/frobtadsappcurses.h
    src/frobtadsappplain.h
    src/frobcurses.h
    src/tadswindow.h
    src/main.cc
    src/oemcurses.c
    src/options.cc
    src/options.h
    src/oscurses.cc
    src/osscurses.cc
)

# TADS 2 runtime sources.
set (
    T2RSOURCES
    tads2/dbgtr.c
    tads2/trd.c
    tads2/execmd.c
    tads2/vocab.c
    tads2/qas.c
    tads2/runstat.c
    tads2/argize.c
    tads2/ply.c
    tads2/linfdum.c
)

# TADS 3 runtime sources.
set (
    T3RSOURCES
    tads3/osifcnet.cpp
    tads3/tcprsnf.cpp
    tads3/tcprsnl.cpp
    tads3/tcprs_rt.cpp
    tads3/tct3_d.cpp
    tads3/tct3nl.cpp
    tads3/unix/osnetunix.cpp
    tads3/vmbifl.cpp
    tads3/vmbifnet.cpp
    tads3/vmbifregn.cpp
    tads3/vmhttpreq.cpp
    tads3/vmhttpsrv.cpp
    tads3/vmmain.cpp
    tads3/vmnetcfg.cpp
    tads3/vmnet.cpp
    tads3/vmnetfil.cpp
    tads3/vmsa.cpp
)

add_executable (
    frob
    ${FROBSOURCES}
    ${COMMONSOURCES}
    ${T2HEADERS}
    ${T2RSOURCES}
    ${T2RCSOURCES}
    ${T3HEADERS}
    ${T3RSOURCES}
    ${T3RCSOURCES}
    ${T3RCSOURCES_ND}
)

target_compile_definitions (
    frob PRIVATE
    RUNTIME
)

target_include_directories (
    frob PRIVATE
    ${CURL_INCLUDE_DIRS}
    ${CURSES_INCLUDE_DIR}
)

target_link_libraries (
    frob
    ${CURL_LIBRARIES}
    ${CURSES_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

if (ENABLE_FROBD)
    add_executable (
        frobd
        ${FROBSOURCES}
        ${COMMONSOURCES}
        ${T2HEADERS}
        ${T2RSOURCES}
        ${T2RCSOURCES}
        ${T3HEADERS}
        ${T3RSOURCES}
        ${T3RCSOURCES}
        ${T3RCSOURCES_D}
        src/debugui.cc
        tads3/vmdbg.cpp
    )

    target_compile_definitions (
        frobd PRIVATE
        RUNTIME VM_DEBUGGER
    )

    target_include_directories (
        frobd PRIVATE
        ${CURL_INCLUDE_DIRS}
        ${CURSES_INCLUDE_DIR}
    )

    target_link_libraries (
        frobd
        ${CURL_LIBRARIES}
        ${CURSES_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
    )
endif(ENABLE_FROBD)

install(TARGETS frob DESTINATION bin)
