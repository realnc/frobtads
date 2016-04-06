# TADS 2 compiler-only sources.
set (
    T2CSOURCES
    src/t2compmain.c
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
)

add_executable (
    tadsc
    ${COMMONSOURCES}
    ${T2HEADERS}
    ${T2RCSOURCES}
    ${T2CSOURCES}
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
