cmake_minimum_required(VERSION 3.1)
project(FrobTADS)

option (
    ENABLE_INTERPRETER
    "Build the interpreter (frob)."
    ON
)

option (
    ENABLE_TADSNET
    "Enable the TADS networking layer."
    ON
)

option (
    ENABLE_T2_COMPILER
    "Build the TADS 2 compiler (tadsc)."
    ON
)

option (
    ENABLE_T3_COMPILER
    "Build the TADS 3 compiler (t3make)."
    ON
)

option (
    ENABLE_T3_DEBUG
    "Build the debug version of TADS 3."
    OFF
)

option (
    ENABLE_T3_TEST_SUITE
    "Build the TADS 3 test suite (requires ENABLE_T3_DEBUG)."
    OFF
)

option (
    ENABLE_T2_RUNTIME_CHECKS
    "Enable TADS2 runtime error checks."
    OFF
)

option (
    ENABLE_FROBD
    "Build frobd, a version of frob usable by debuggers."
    OFF
)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.13")
    cmake_policy(SET CMP0076 NEW)
endif()

set(FROB_VERSION "1.2.3")
set(FROB_MAINTAINER "Nikos Chantziaras")
set(FROB_BUG_REPORT_EMAIL "realnc@gmail.com")
set(TADS_OEM_VERSION "1")

set(T2_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/frobtads/tads2")
set(T3_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/frobtads/tads3")
set(T3_INC_DIR "${T3_INSTALL_DIR}/include")
set(T3_LIB_DIR "${T3_INSTALL_DIR}/lib")
set(T3_RES_DIR "${T3_INSTALL_DIR}/res")
set(T3_LOG_FILE "/tmp/frob.log")

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(TestBigEndian)
include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckCXXSourceCompiles)
include(CheckTypeSize)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX
        OR CMAKE_C_COMPILER_ID MATCHES "Clang"
        OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing")
endif()

add_definitions (
    # Tells the base code to include our osfrobtads.h header.
    -DFROBTADS

    # Instructs the TADS 3 compiler to generate T3VM bytecode. This is also
    # needed by the runtime, since it does dynamic compiling.
    -DTC_TARGET_T3

    # Tells the T3VM to use the "flat" pool manager. A flat pool is somewhat
    # faster than the default paged pool normaly used by the VM, but lacks
    # dynamic memory capabilites. The paged pool is only useful for debuggers
    # though, so we choose a flat pool. (See tads3/vmpoolsl.h for details.)
    #-D VM_FLAT_POOL

    # Needed to get POSIX-style stat() and lstat().
    -D_SVID_SOURCE
    -D_DEFAULT_SOURCE
)

if (ENABLE_TADSNET)
    add_definitions (
        # Enable the TADS 3 networking layer.
        -DTADSNET
    )
endif()


check_function_exists(mkdir HAVE_MKDIR)
if (NOT HAVE_MKDIR)
    check_function_exists(_mkdir HAVE_UNDERSCORE_MKDIR)
endif()
check_function_exists(memicmp HAVE_MEMICMP)
check_function_exists(putenv HAVE_PUTENV)
check_function_exists(stricmp HAVE_STRICMP)
check_function_exists(strnicmp HAVE_STRNICMP)
check_function_exists(strcasecmp HAVE_STRCASECMP)
check_function_exists(strncasecmp HAVE_STRNCASECMP)
check_function_exists(chdir HAVE_CHDIR)
if (NOT HAVE_CHDIR)
    check_function_exists(SetCurrentDirectory HAVE_SETCURRENTDIRECTORY)
endif()
check_function_exists(setlocale HAVE_SETLOCALE)

check_include_files(sys/time.h HAVE_SYS_TIME_H)
check_include_files(glob.h HAVE_GLOB_H)
check_include_files(locale.h HAVE_LOCALE_H)

# See if time and sys/time can both be included.
check_cxx_source_compiles (
    "#include <sys/time.h>
    #include <time.h>
    int main() {}"
    TIME_WITH_SYS_TIME
)

# Locale support functions.
check_cxx_source_compiles (
    "#include <langinfo.h>
    int main() {char* cs = nl_langinfo(CODESET);}"
    HAVE_LANGINFO_CODESET
)

# Some of the TADS base code tries to define "uchar", "ushort" and the like.
# This will cause a compiler error if these types are already defined, so it
# checks for the OS_U*_DEFINED macros first. We check for the existence of
# these types in both C and C++; the "common.h" header will then define the
# final OS_*_DEFINED macros according to whether it's being compiled by C or
# C++.
check_type_size(uchar C_UCHAR LANGUAGE C)
check_type_size(ushort C_USHORT LANGUAGE C)
check_type_size(uint C_UINT LANGUAGE C)
check_type_size(ulong C_ULONG LANGUAGE C)
check_type_size(uchar CXX_UCHAR LANGUAGE CXX)
check_type_size(ushort CXX_USHORT LANGUAGE CXX)
check_type_size(uint CXX_UINT LANGUAGE CXX)
check_type_size(ulong CXX_ULONG LANGUAGE CXX)

# Some 3rd-party code bundled in TADS 3 uses the CPU_IS_BIGENDIAN and
# ARCH_IS_BIG_ENDIAN macros to decide on endianness, so we set both.
test_big_endian(CPU_IS_BIGENDIAN)
if (CPU_IS_BIGENDIAN)
    add_definitions(-D_M_PPC)
    set(ARCH_IS_BIG_ENDIAN 1)
else()
    add_definitions(-D_M_LE_C11)
    set(ARCH_IS_BIG_ENDIAN 0)
endif()

if (ENABLE_T3_DEBUG)
    # When building TADS 3 in debug mode, the global variables configuration
    # cannot be used.
    add_definitions(-DT3_DEBUG -DVMGLOB_PARAM)
else()
    # Non-debug TADS 3 can use individual external variables for its globals,
    # which is the fastest configuration. (See tads3/vmglob.h for details.)
    add_definitions(-DVMGLOB_VARS)
endif()

if (NOT ENABLE_T2_RUNTIME_CHECKS)
    add_definitions(-DRUNFAST)
endif()

include_directories (
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/tads2
    ${PROJECT_SOURCE_DIR}/tads3
    ${PROJECT_SOURCE_DIR}/tads3/unix
)

include(CMCommon.cmake)

if (ENABLE_INTERPRETER)
    include(CMFrob.cmake)
endif()

if (ENABLE_T2_COMPILER)
    include(CMT2Compiler.cmake)
endif()

if (ENABLE_T3_COMPILER)
    include(CMT3Compiler.cmake)
    if (ENABLE_T3_DEBUG AND ENABLE_T3_TEST_SUITE)
        include(CMTestsuite.cmake)
    endif()
endif()

configure_file (
    ${PROJECT_SOURCE_DIR}/frob_config.h.in
    ${PROJECT_BINARY_DIR}/frob_config.h
)
