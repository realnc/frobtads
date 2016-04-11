add_executable (
    tadsc
    ${TADS2_HEADERS}
    src/osportable.cc
    src/t2compmain.c
    tads2/osgen3.c
    tads2/tcd.c
    tads2/tcgdum.c
    tads2/linf.c
    tads2/tok.c
    tads2/tokth.c
    tads2/fiowrt.c
    tads2/objcomp.c
    tads2/sup.c
    tads2/voccomp.c
    tads2/prscomp.c
    tads2/prs.c
    tads2/emt.c
    tads2/dbg.c
    $<TARGET_OBJECTS:COMMON_OBJECTS>
    $<TARGET_OBJECTS:TADS2_RC_OBJECTS>
)

add_executable (
    tadsrsc
    src/missing.cc
    src/osportable.cc
    tads2/tadsrsc.c
    tads2/osnoui.c
    tads2/osterm.c
)

install (
    FILES
    tads2/adv.t tads2/std.t tads2/gameinfo.t
    DESTINATION share/frobtads/tads2/
)
