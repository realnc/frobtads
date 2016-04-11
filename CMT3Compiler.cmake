add_executable (
    t3make
    ${TADS3_HEADERS}
    src/osportable.cc
    src/ost3comp.cc
    tads3/os_stdio.cpp
    tads3/rcmain.cpp
    tads3/std_dbg.cpp
    tads3/tcgenfil.cpp
    tads3/tcmakecl.cpp
    tads3/tcmake.cpp
    tads3/tcprsfil.cpp
    tads3/tcprsimg.cpp
    tads3/tcprsprg.cpp
    tads3/tct3img.cpp
    tads3/tct3prg.cpp
    tads3/vmbifc.cpp
    tads3/vmbifreg.cpp
    tads3/vmhttpdum.cpp
    tads3/vmimgrb.cpp
    tads3/vmpreini.cpp
    tads3/vmwrtimg.cpp
    tads3/vmrun.cpp
    $<TARGET_OBJECTS:COMMON_OBJECTS>
    $<TARGET_OBJECTS:TADS3_RC_OBJECTS>
    $<TARGET_OBJECTS:TADS3_RC_OBJECTS_ND>
)
target_include_directories (
    t3make PRIVATE
    ${PROJECT_SOURCE_DIR}/tads3/test
)

install (
    DIRECTORY tads3/include/
    DESTINATION ${T3_INC_DIR}
)

install (
    DIRECTORY tads3/lib/
    DESTINATION ${T3_LIB_DIR}
)

install (
    DIRECTORY tads3/doc/
    DESTINATION ${T3_INSTALL_DIR}/doc
)

add_custom_target(
    sample
    COMMAND "${CMAKE_COMMAND}" -E remove_directory samples
    COMMAND "${CMAKE_COMMAND}" -E make_directory samples/obj
    COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_SOURCE_DIR}/tads3/samples/sample.t3m" samples
    COMMAND "${CMAKE_COMMAND}" -E chdir samples "$<TARGET_FILE:t3make>" -a -al
            -f sample.t3m
            -FL "${CMAKE_CURRENT_SOURCE_DIR}/tads3/lib"
            -FI "${CMAKE_CURRENT_SOURCE_DIR}/tads3/include"
            -Fs "${CMAKE_CURRENT_SOURCE_DIR}/tads3/samples"
            -I "${CMAKE_CURRENT_SOURCE_DIR}/tads3/samples"
            -Fy samples/obj -Fo samples/obj
    DEPENDS t3make
    COMMENT "Compiling the TADS 3 sample game"
    VERBATIM
)
