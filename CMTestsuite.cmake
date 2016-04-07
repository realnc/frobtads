include(CTest)
enable_testing()

set(T3_DAT "${CMAKE_CURRENT_SOURCE_DIR}/tads3/test/data")
set(T3_LOG "${CMAKE_CURRENT_SOURCE_DIR}/tads3/test/log")
set(T3_OUT "${CMAKE_CURRENT_BINARY_DIR}/test/out")
set(T3_RESDIR "${CMAKE_CURRENT_SOURCE_DIR}/tads3")
set(T3_INCDIR "${CMAKE_CURRENT_SOURCE_DIR}/tads3/include")
set(T3_LIBDIR "${CMAKE_CURRENT_SOURCE_DIR}/tads3/lib")
set(SCRIPTS "${CMAKE_CURRENT_SOURCE_DIR}/testscripts")
set(TESTPROGS "${CMAKE_CURRENT_BINARY_DIR}/test")
set(T3_MAKE "$<TARGET_FILE:t3make>")

macro(set_test_env test)
set_tests_properties(${test} PROPERTIES ENVIRONMENT
"T3_DAT=${T3_DAT};\
T3_LOG=${T3_LOG};\
T3_OUT=${T3_OUT};\
T3_RESDIR=${T3_RESDIR};\
T3_INCDIR=${T3_INCDIR};\
T3_LIBDIR=${T3_LIBDIR};\
SCRIPTS=${SCRIPTS};\
TESTPROGS=${TESTPROGS};\
t3make=${T3_MAKE};"
)
endmacro()

macro(run_test test_type test_name script_file)
    add_test (
        NAME ${test_type}-${test_name}
        COMMAND "${SCRIPTS}/${script_file}" ${ARGV3} ${ARGV4} ${ARGV5} ${ARGV6} ${ARGV7} ${ARGV8} ${ARGV9}
                ${ARGV10} ${ARGV11}
    )
    set_test_env(${test_type}-${test_name})
endmacro()

macro(run_preproc_test test)
    run_test(PreprocessorTest ${test} test_pp.sh ${test})
endmacro()

macro(run_exec_test test codepage)
    run_test(ExecutionTest ${test} test_ex.sh ${test} ${codepage})
endmacro()

macro(run_make_test test)
    run_test(MakeTest ${test} test_make.sh ${ARGV1} ${ARGV2} ${ARGV3} ${ARGV4} ${ARGV5} ${ARGV6} ${ARGV7}
        ${ARGV8} ${ARGV9})
endmacro()

macro(run_restore_test test arg1 arg2)
    run_test(RestoreTest ${test} test_restore.sh ${arg1} ${arg2})
endmacro()

macro(run_preinit_test test)
    run_test(PreinitTest ${test} test_pre.sh ${test})
endmacro()

add_test (
    NAME ObjectSubsystem
    COMMAND test_obj
)

foreach(test ansi circ circ2 embed define ifdef concat varmacpp)
    run_preproc_test(${test})
endforeach()

foreach(test basic finally dstr fnredef undo gotofin)
    run_exec_test(${test} cp437)
endforeach()

run_exec_test(builtin latin1)

foreach(test asi anon anonfunc anonobj anonvarg badnest bignum bignum2 findreplace foreach funcparm htmlify
        ifnil inh_next isin join lclprop listprop lookup lookup2 lookup3 lookupdef multidyn nested newprop
        objloop opoverload propaddr propptr rexassert rexreplace setsc shr spec2html spec2text split sprintf
        strcomp2 strbuf substr unicode varmac vector vector2 vector3 testaddr2 testaddr3 testaddr4 strtpl
        listminmax packstr packarr)
    run_make_test(${test} cp437 ${test} ${test})
endforeach()

run_make_test(strcomp3 latin1 strcomp3 strcomp3)

foreach(test catch save html addlist listpar arith)
    run_make_test(${test} cp437 -nodef ${test} ${test})
endforeach()

foreach(test extfunc objrep funcrep conflict)
    run_make_test(${test} cp437 -nodef ${test} ${test}1 ${test}2)
endforeach()

run_make_test(vocext cp437 -pre vocext vocext1 vocext2 reflect)

foreach(test extern objmod)
    run_make_test(${test} cp437 -nodef ${test} ${test}1 ${test}2 ${test}3)
endforeach()

run_make_test(gram2 cp437 -nodef gram2 tok gram2)

run_make_test(rand cp437 -nodef rand rand)

run_make_test(stack cp437 -debug stack stack reflect)

run_make_test(targprop cp437 -pre targprop targprop reflect)

run_make_test(clone cp437 -pre clone clone reflect)

run_make_test(printexpr cp437 -pre printexpr printexpr dynfunc)

run_make_test(dynctx cp437 -pre dynctx dynctx dynfunc)

foreach(test vec_pre symtab enumprop modtobj undef undef2 newembed newembederr triplequote optargs
        optargs_err optargs_err2)
    run_make_test(${test} cp437 -pre ${test} ${test})
endforeach()

# Disable this for now; they can't work since they don't support building
# in a different directory than where the sources reside.
#run_make_test(multimethod_dynamic -pre multimethod_dynamic multimethod multmethmultimethod_static
#              -DMULTMETH_STATIC_INHERITED multimethod multmeth)

run_make_test(multimethod_dynamic2 cp437 -pre multimethod_dynamic2 multimethod multimethod2 multmeth)

run_make_test(multimethod_static2 cp437 -pre multimethod_static2 -DMULTMETH_STATIC_INHERITED
    multimethod multimethod2 multmeth)

run_make_test(namedparam cp437 -pre namedparam namedparam multmeth)

run_make_test(bifptr cp437 -pre bifptr bifptr dynfunc)

run_make_test(setmethod cp437 -pre setmethod setmethod dynfunc)

run_make_test(lclvars cp437 -pre lclvars lclvars lclvars2 reflect dynfunc)

run_make_test(dynamicGrammar cp437 -pre dynamicGrammar dynamicGrammar tok dynfunc gramprod)

run_make_test(iter cp437 iter iter)
run_restore_test(iter iter2 iter)

run_preinit_test(preinit)

# Extra compiler sources for dynamic code compilation in interpreter.
set (
    DYN_COMP_OBJS
    tads3/vmrunsym.cpp
    tads3/tcprs.cpp
    tads3/tcprs_rt.cpp
    tads3/tcprsnf.cpp
    tads3/tcprsstm.cpp
    tads3/tcprsnl.cpp
    tads3/tcgen.cpp
    tads3/tcglob.cpp
    tads3/tcerr.cpp
    tads3/tcerrmsg.cpp
    tads3/tctok.cpp
    tads3/tcmain.cpp
    tads3/tcsrc.cpp
    tads3/tchostsi.cpp
    tads3/tclibprs.cpp
    tads3/tccmdutl.cpp
)

set(REG_BUILTIN_CHAR  tads3/vmbifreg.cpp)
set(REG_BUILTIN_CHARNET tads3/vmbifregn.cpp)
set(REG_BUILTIN_HTML tads3/vmbifregx.cpp)
set(REG_BUILTIN_HTMLNET tads3/vmbifregxn.cpp)
set(REG_METACLASS tads3/vmmcreg.cpp)

set (
    TARGET_OBJS_BASE
    tads3/tct3.cpp
    tads3/tct3stm.cpp
    tads3/tct3unas.cpp
)

set (
    TARGET_OBJS_NO_RT
    ${TARGET_OBJS_BASE}
    tads3/tct3nl.cpp
    tads3/tct3_d.cpp
)

set (
    TARGET_OBJS_NO_LINK
    ${TARGET_OBJS_BASE}
    tads3/tct3nl.cpp
    tads3/tct3_d.cpp
)

set (
    TARGET_OBJS_SYM
    ${TARGET_OBJS_BASE}
    tads3/tct3nl.cpp
    tads3/tct3prg.cpp
)

set (
    TARGET_OBJS
    ${TARGET_OBJS_BASE}
    tads3/tct3img.cpp
    tads3/tct3prg.cpp
)

add_executable (
    t3pre
    tads3/test/test_pre.cpp
    src/osportable.cc
    src/missing.cc
    src/ost3comp.cc
    tads3/std.cpp
    tads3/std_dbg.cpp
    tads3/charmap.cpp
    tads3/resload.cpp
    tads3/resldexe.cpp
    tads3/vmwrtimg.cpp
    tads3/vminit.cpp
    tads3/vmini_nd.cpp
    tads3/vminitim.cpp
    tads3/vmcfgmem.cpp
    tads3/vmobj.cpp
    tads3/vmundo.cpp
    tads3/vmtobj.cpp
    tads3/vmpat.cpp
    tads3/vmstrcmp.cpp
    tads3/vmdict.cpp
    tads3/vmgram.cpp
    tads3/vmstr.cpp
    tads3/vmcoll.cpp
    tads3/vmiter.cpp
    tads3/vmfref.cpp
    tads3/vmlst.cpp
    tads3/vmsort.cpp
    tads3/vmsortv.cpp
    tads3/vmbignum.cpp
    tads3/vmvec.cpp
    tads3/vmintcls.cpp
    tads3/vmanonfn.cpp
    tads3/vmlookup.cpp
    tads3/vmstrbuf.cpp
    tads3/vmdynfunc.cpp
    tads3/vmbytarr.cpp
    tads3/vmcset.cpp
    tads3/vmfilobj.cpp
    tads3/vmtmpfil.cpp
    tads3/vmpack.cpp
    tads3/vmhttpdum.cpp
    tads3/vmnetfillcl.cpp
    tads3/vmstack.cpp
    tads3/vmerr.cpp
    tads3/vmerrmsg.cpp
    tads3/vmpool.cpp
    tads3/vmpoolim.cpp
    tads3/vmtype.cpp
    tads3/vmtypedh.cpp
    tads3/utf8.cpp
    tads3/vmglob.cpp
    tads3/vmrun.cpp
    tads3/vmfunc.cpp
    tads3/vmmeta.cpp
    tads3/vmpreini.cpp
    tads3/vmimgrb.cpp
    tads3/vmbif.cpp
    tads3/vmbifc.cpp
    tads3/vmimage.cpp
    tads3/vmimg_nd.cpp
    tads3/vmsrcf.cpp
    tads3/vmfile.cpp
    tads3/vmbiftad.cpp
    tads3/vmisaac.cpp
    tads3/vmbiftio.cpp
    tads3/askf_tx3.cpp
    tads3/indlg_tx3.cpp
    tads3/vmsave.cpp
    tads3/vmcrc.cpp
    tads3/vmbift3.cpp
    tads3/vmbt3_nd.cpp
    tads3/vmregex.cpp
    tads3/vmconsol.cpp
    tads3/vmconmor.cpp
    tads3/vmconhmp.cpp
    tads3/os_stdio.cpp
    tads3/vmhosttx.cpp
    tads3/vmhostsi.cpp
    tads3/vmhash.cpp
    tads3/sha2.cpp
    tads3/md5.cpp
    tads3/vmlog.cpp
    tads3/vmbignumlib.cpp
    tads3/vmtz.cpp
    tads3/vmtzobj.cpp
    tads3/vmdate.cpp
    tads3/vmfilnam.cpp
    tads3/vmop.cpp
    ${DYN_COMP_OBJS}
    ${REG_METACLASS}
    ${REG_BUILTIN_CHAR}
    tads3/derived/vmuni_cs.cpp
    tads2/osifc.c
    tads2/osnoui.c
    tads2/osrestad.c
    tads2/osstzprs.c
    tads2/ostzposix.c
    ${TARGET_OBJS_NO_LINK}
)

add_executable (
    test_utf8
    tads3/test/test_utf8.cpp
    tads3/utf8.cpp
)

add_executable (
    test_chr
    tads3/test/test_chr.cpp
    src/osportable.cc
    src/missing.cc
    tads2/osnoui.c
    tads2/osstzprs.c
    tads3/std.cpp
    tads3/std_dbg.cpp
    tads3/utf8.cpp
    tads3/charmap.cpp
    tads3/resload.cpp
    tads3/resnoexe.cpp
    tads3/derived/vmuni_cs.cpp
)

add_executable (
    test_gets
    tads3/test/test_gets.cpp
    src/osportable.cc
    src/missing.cc
    tads2/osnoui.c
    tads2/osstzprs.c
    tads3/std.cpp
    tads3/std_dbg.cpp
    tads3/utf8.cpp
    tads3/charmap.cpp
    tads3/resload.cpp
    tads3/resnoexe.cpp
    tads3/tcsrc.cpp
    tads3/derived/vmuni_cs.cpp
)

add_executable (
    test_pool
    tads3/test/test_pool.cpp
    tads3/std.cpp
    tads3/std_dbg.cpp
    tads3/vmpool.cpp
    tads3/vmpoolim.cpp
    tads3/vmerr.cpp
    tads3/vmerrmsg.cpp
    tads3/vmglob.cpp
    tads3/derived/vmuni_cs.cpp
)

add_executable (
    test_err
    tads3/test/test_err.cpp
    tads3/std.cpp
    tads3/std_dbg.cpp
    tads3/vmerr.cpp
    tads3/vmerrmsg.cpp
    tads3/derived/vmuni_cs.cpp
)

add_executable (
    test_regex
    tads3/test/test_regex.cpp
    src/missing.cc
    tads3/vmregex.cpp
    tads3/vmerr.cpp
    tads3/vmerrmsg.cpp
    tads3/derived/vmuni_cs.cpp
    tads3/std.cpp
    tads3/std_dbg.cpp
)

add_executable (
    test_obj
    tads3/test/test_obj.cpp
    src/osportable.cc
    src/missing.cc
    src/ost3comp.cc
    tads3/std.cpp
    tads3/std_dbg.cpp
    tads3/charmap.cpp
    tads3/resload.cpp
    tads3/resnoexe.cpp
    tads3/vmfile.cpp
    tads3/vmundo.cpp
    tads3/vmobj.cpp
    tads3/vmtobj.cpp
    tads3/vmpat.cpp
    tads3/vmstrcmp.cpp
    tads3/vmstr.cpp
    tads3/vmcoll.cpp
    tads3/vmiter.cpp
    tads3/vmfref.cpp
    tads3/vmlst.cpp
    tads3/vmsort.cpp
    tads3/vmsortv.cpp
    tads3/vmbignum.cpp
    tads3/vmsave.cpp
    tads3/vmcrc.cpp
    tads3/vmvec.cpp
    tads3/vmintcls.cpp
    tads3/vmanonfn.cpp
    tads3/vmlookup.cpp
    tads3/vmstrbuf.cpp
    tads3/vmdynfunc.cpp
    tads3/vmbytarr.cpp
    tads3/vmcset.cpp
    tads3/vmfilobj.cpp
    tads3/vmtmpfil.cpp
    tads3/vmpack.cpp
    tads3/vmstack.cpp
    tads3/vmdict.cpp
    tads3/vmgram.cpp
    tads3/vmhttpsrv.cpp
    tads3/vmhttpreq.cpp
    tads3/vmnet.cpp
    tads3/vmnetui.cpp
    tads3/vmnetcfg.cpp
    tads3/vmnetfil.cpp
    tads3/vmnetfillcl.cpp
    tads3/unix/osnetunix.cpp
    tads3/osifcnet.cpp
    tads3/vmrefcnt.cpp
    tads3/sha2.cpp
    tads3/md5.cpp
    tads3/vmerr.cpp
    tads3/vmerrmsg.cpp
    tads3/vmpool.cpp
    tads3/vmpoolim.cpp
    tads3/vmtype.cpp
    tads3/vmtypedh.cpp
    tads3/utf8.cpp
    tads3/vmglob.cpp
    tads3/vmrun.cpp
    tads3/vmsrcf.cpp
    tads3/vmfunc.cpp
    tads3/vmmeta.cpp
    tads3/vmbif.cpp
    tads3/vmsa.cpp
    tads3/vmbifl.cpp
    tads3/vmbiftad.cpp
    tads3/vmisaac.cpp
    tads3/vmbiftio.cpp
    tads3/vmbifnet.cpp
    tads3/askf_tx3.cpp
    tads3/indlg_tx3.cpp
    tads3/vmbift3.cpp
    tads3/vmbt3_nd.cpp
    tads3/vminit.cpp
    tads3/vmini_nd.cpp
    tads3/vmconsol.cpp
    tads3/vmconmor.cpp
    tads3/vmconhmp.cpp
    tads3/os_stdio.cpp
    tads3/vminitim.cpp
    tads3/vmcfgmem.cpp
    tads3/vmregex.cpp
    tads3/vmhosttx.cpp
    tads3/vmhostsi.cpp
    tads3/vmhash.cpp
    ${REG_METACLASS}
    ${REG_BUILTIN_CHARNET}
    tads3/derived/vmuni_cs.cpp
    tads3/tcprs.cpp
    tads3/tcprsnl.cpp
    tads3/tcprsfil.cpp
    tads3/tcprsstm.cpp
    tads3/tcprsprg.cpp
    tads3/tcgen.cpp
    tads3/tcgenfil.cpp
    tads3/tcglob.cpp
    tads3/tcerr.cpp
    tads3/tcerrmsg.cpp
    tads3/tctok.cpp
    tads3/tcmain.cpp
    tads3/tcsrc.cpp
    tads3/tchostsi.cpp
    tads3/tclibprs.cpp
    tads3/tccmdutl.cpp
    tads3/vmrunsym.cpp
    tads3/vmlog.cpp
    tads3/vmbignumlib.cpp
    tads3/vmfilnam.cpp
    tads3/vmdate.cpp
    tads3/vmtz.cpp
    tads3/vmtzobj.cpp
    tads3/vmop.cpp
    ${TARGET_OBJS_NO_LINK}
    tads3/tct3prg.cpp
    tads2/osifc.c
    tads2/osnoui.c
    tads2/osrestad.c
    tads2/osstzprs.c
    tads2/ostzposix.c
)

target_link_libraries (
    test_obj
    ${CURL_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

add_executable (
    test_sort
    tads3/test/test_sort.cpp
    tads3/vmsort.cpp
)

add_executable (
    test_write
    tads3/test/test_write.cpp
    tads3/std.cpp
    tads3/std_dbg.cpp
    tads3/vmwrtimg.cpp
    tads3/vmfile.cpp
    tads3/vmerr.cpp
    tads3/vmerrmsg.cpp
    tads3/vmtypedh.cpp
    tads3/derived/vmuni_cs.cpp
)

add_executable (
    test_exec
    tads3/test/test_exec.cpp
    src/missing.cc
    src/osportable.cc
    src/ost3comp.cc
    src/teststubs.c
    tads3/vmmain.cpp
    tads3/std.cpp
    tads3/std_dbg.cpp
    tads3/charmap.cpp
    tads3/resload.cpp
    tads3/resldexe.cpp
    tads3/vminit.cpp
    tads3/vmini_nd.cpp
    tads3/vmconsol.cpp
    tads3/vmconmor.cpp
    tads3/vmconhmp.cpp
    tads3/os_stdio.cpp
    tads3/vminitim.cpp
    tads3/vmcfgmem.cpp
    tads3/vmobj.cpp
    tads3/vmundo.cpp
    tads3/vmtobj.cpp
    tads3/vmpat.cpp
    tads3/vmstrcmp.cpp
    tads3/vmstr.cpp
    tads3/vmcoll.cpp
    tads3/vmiter.cpp
    tads3/vmfref.cpp
    tads3/vmlst.cpp
    tads3/vmsort.cpp
    tads3/vmsortv.cpp
    tads3/vmbignum.cpp
    tads3/vmvec.cpp
    tads3/vmintcls.cpp
    tads3/vmanonfn.cpp
    tads3/vmdict.cpp
    tads3/vmgram.cpp
    tads3/vmlookup.cpp
    tads3/vmstrbuf.cpp
    tads3/vmdynfunc.cpp
    tads3/vmbytarr.cpp
    tads3/vmcset.cpp
    tads3/vmfilobj.cpp
    tads3/vmtmpfil.cpp
    tads3/vmpack.cpp
    tads3/vmhttpsrv.cpp
    tads3/vmhttpreq.cpp
    tads3/vmnet.cpp
    tads3/vmnetui.cpp
    tads3/vmnetcfg.cpp
    tads3/vmnetfil.cpp
    tads3/vmnetfillcl.cpp
    tads3/unix/osnetunix.cpp
    tads3/osifcnet.cpp
    tads3/vmrefcnt.cpp
    tads3/sha2.cpp
    tads3/md5.cpp
    tads3/vmstack.cpp
    tads3/vmerr.cpp
    tads3/vmerrmsg.cpp
    tads3/vmpool.cpp
    tads3/vmpoolim.cpp
    tads3/vmtype.cpp
    tads3/vmtypedh.cpp
    tads3/utf8.cpp
    tads3/vmglob.cpp
    tads3/vmrun.cpp
    tads3/vmfunc.cpp
    tads3/vmmeta.cpp
    tads3/vmsa.cpp
    tads3/vmbif.cpp
    tads3/vmbifl.cpp
    tads3/vmimage.cpp
    tads3/vmimg_nd.cpp
    tads3/vmsrcf.cpp
    tads3/vmfile.cpp
    tads3/vmbiftad.cpp
    tads3/vmisaac.cpp
    tads3/vmbiftio.cpp
    tads3/askf_tx3.cpp
    tads3/indlg_tx3.cpp
    tads3/vmsave.cpp
    tads3/vmcrc.cpp
    tads3/vmbift3.cpp
    tads3/vmbt3_nd.cpp
    tads3/vmbifnet.cpp
    tads3/vmregex.cpp
    tads3/vmhosttx.cpp
    tads3/vmhostsi.cpp
    tads3/vmhash.cpp
    tads3/vmbignumlib.cpp
    tads3/vmfilnam.cpp
    tads3/vmdate.cpp
    tads3/vmtz.cpp
    tads3/vmtzobj.cpp
    tads3/vmop.cpp
    ${REG_METACLASS}
    ${REG_BUILTIN_CHARNET}
    tads3/derived/vmuni_cs.cpp
    tads3/vmlog.cpp
    ${DYN_COMP_OBJS}
    tads2/osifc.c
    tads2/osnoui.c
    tads2/osrestad.c
    tads2/osstzprs.c
    tads2/ostzposix.c
    ${TARGET_OBJS_NO_LINK}
)

target_link_libraries (
    test_exec
    ${CURL_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

add_executable (
    test_tok
    tads3/test/test_tok.cpp
    src/osportable.cc
    src/missing.cc
    tads3/std.cpp
    tads3/std_dbg.cpp
    tads3/tcglob.cpp
    tads3/vmerr.cpp
    tads3/vmerrmsg.cpp
    tads3/utf8.cpp
    tads3/charmap.cpp
    tads3/resload.cpp
    tads3/resnoexe.cpp
    tads3/tcmain.cpp
    tads3/tcerr.cpp
    tads3/tcerrmsg.cpp
    tads3/tchostsi.cpp
    tads3/tcsrc.cpp
    tads3/tctok.cpp
    tads3/tcprs.cpp
    tads3/tcprsnl.cpp
    tads3/tcprsnf.cpp
    tads3/tcgen.cpp
    tads3/tct3.cpp
    tads3/tct3unas.cpp
    tads3/tct3nl.cpp
    tads3/tct3_d.cpp
    tads3/vmhash.cpp
    tads3/vmtypedh.cpp
    tads3/vmbignumlib.cpp
    tads3/vmop.cpp
    tads3/derived/vmuni_cs.cpp
    tads2/osnoui.c
    tads2/osstzprs.c
)

add_executable (
    test_prs
    tads3/test/test_prs.cpp
    src/osportable.cc
    src/missing.cc
    tads3/tcglob.cpp
    tads3/std.cpp
    tads3/std_dbg.cpp
    tads3/vmerr.cpp
    tads3/vmerrmsg.cpp
    tads3/utf8.cpp
    tads3/charmap.cpp
    tads3/resload.cpp
    tads3/resnoexe.cpp
    tads3/tcmain.cpp
    tads3/tcerr.cpp
    tads3/tcerrmsg.cpp
    tads3/tchostsi.cpp
    tads3/tcsrc.cpp
    tads3/tctok.cpp
    tads3/tcprs.cpp
    tads3/tcprsfil.cpp
    tads3/tcprsstm.cpp
    tads3/tcprsprg.cpp
    tads3/tcprsimg.cpp
    tads3/tcgen.cpp
    tads3/tcgenfil.cpp
    ${TARGET_OBJS}
    tads3/vmhash.cpp
    tads3/vmwrtimg.cpp
    tads3/vmtypedh.cpp
    tads3/vmfile.cpp
    tads2/osnoui.c
    tads3/vmglob.cpp
    tads3/derived/vmuni_cs.cpp
    tads3/vmbignumlib.cpp
    tads3/vmop.cpp
    tads2/osstzprs.c
)

add_executable (
    test_sym
    tads3/test/test_sym.cpp
    src/osportable.cc
    src/missing.cc
    tads3/tcglob.cpp
    tads3/std.cpp
    tads3/std_dbg.cpp
    tads3/vmerr.cpp
    tads3/vmerrmsg.cpp
    tads3/utf8.cpp
    tads3/charmap.cpp
    tads3/resload.cpp
    tads3/resnoexe.cpp
    tads3/tcmain.cpp
    tads3/tcerr.cpp
    tads3/tcerrmsg.cpp
    tads3/tchostsi.cpp
    tads3/tcsrc.cpp
    tads3/tctok.cpp
    tads3/tcprs.cpp
    tads3/tcprsfil.cpp
    tads3/tcprsnl.cpp
    ${TARGET_OBJS_SYM}
    tads3/tcprsstm.cpp
    tads3/tcprsprg.cpp
    tads3/tcgen.cpp
    tads3/tcgenfil.cpp
    tads3/vmhash.cpp
    tads3/vmtypedh.cpp
    tads3/vmfile.cpp
    tads3/derived/vmuni_cs.cpp
    tads3/vmbignumlib.cpp
    tads3/vmglob.cpp
    tads3/vmop.cpp
    tads2/osnoui.c
    tads2/osstzprs.c
)

add_executable (
    test_prs_top
    tads3/test/test_prs_top.cpp
    src/osportable.cc
    src/missing.cc
    tads3/tcglob.cpp
    tads3/std.cpp
    tads3/std_dbg.cpp
    tads3/vmerr.cpp
    tads3/vmerrmsg.cpp
    tads3/utf8.cpp
    tads3/charmap.cpp
    tads3/resload.cpp
    tads3/resnoexe.cpp
    tads3/tcmain.cpp
    tads3/tcerr.cpp
    tads3/tcerrmsg.cpp
    tads3/tchostsi.cpp
    tads3/tcsrc.cpp
    tads3/tctok.cpp
    tads3/tcprs.cpp
    tads3/tcprsfil.cpp
    tads3/tcprsstm.cpp
    tads3/tcprsprg.cpp
    tads3/tcprsimg.cpp
    tads3/tcgen.cpp
    tads3/tcgenfil.cpp
    ${TARGET_OBJS}
    tads3/vmhash.cpp
    tads3/vmwrtimg.cpp
    tads3/vmtypedh.cpp
    tads3/vmfile.cpp
    tads3/vmglob.cpp
    tads3/derived/vmuni_cs.cpp
    tads3/vmbignumlib.cpp
    tads3/vmop.cpp
    tads2/osnoui.c
    tads2/osstzprs.c
)

add_executable (
    test_comp_obj
    tads3/test/test_comp_obj.cpp
    src/osportable.cc
    src/missing.cc
    tads3/tcglob.cpp
    tads3/std.cpp
    tads3/std_dbg.cpp
    tads3/vmerr.cpp
    tads3/vmerrmsg.cpp
    tads3/utf8.cpp
    tads3/charmap.cpp
    tads3/resload.cpp
    tads3/resnoexe.cpp
    tads3/tcmain.cpp
    tads3/tcerr.cpp
    tads3/tcerrmsg.cpp
    tads3/tchostsi.cpp
    tads3/tcsrc.cpp
    tads3/tctok.cpp
    tads3/tcprs.cpp
    tads3/tcprsfil.cpp
    tads3/tcprsstm.cpp
    tads3/tcprsprg.cpp
    tads3/tcprsimg.cpp
    tads3/tcgen.cpp
    tads3/tcgenfil.cpp
    ${TARGET_OBJS}
    tads3/vmhash.cpp
    tads3/vmwrtimg.cpp
    tads3/vmtypedh.cpp
    tads3/vmfile.cpp
    tads3/vmglob.cpp
    tads3/derived/vmuni_cs.cpp
    tads3/vmbignumlib.cpp
    tads3/vmop.cpp
    tads2/osnoui.c
    tads2/osstzprs.c
)

add_executable (
    test_link
    tads3/test/test_link.cpp
    src/osportable.cc
    src/missing.cc
    tads3/tcglob.cpp
    tads3/std.cpp
    tads3/std_dbg.cpp
    tads3/vmerr.cpp
    tads3/vmerrmsg.cpp
    tads3/utf8.cpp
    tads3/charmap.cpp
    tads3/resload.cpp
    tads3/resnoexe.cpp
    tads3/tcmain.cpp
    tads3/tcerr.cpp
    tads3/tcerrmsg.cpp
    tads3/tchostsi.cpp
    tads3/tcsrc.cpp
    tads3/tctok.cpp
    tads3/tcprs.cpp
    tads3/tcprsfil.cpp
    tads3/tcprsstm.cpp
    tads3/tcprsprg.cpp
    tads3/tcprsimg.cpp
    tads3/tcgen.cpp
    tads3/tcgenfil.cpp
    ${TARGET_OBJS}
    tads3/vmhash.cpp
    tads3/vmwrtimg.cpp
    tads3/vmtypedh.cpp
    tads3/vmfile.cpp
    tads3/vmglob.cpp
    tads3/derived/vmuni_cs.cpp
    tads3/vmbignumlib.cpp
    tads3/vmop.cpp
    tads2/osnoui.c
    tads2/osstzprs.c
)

set_target_properties(
    t3pre test_utf8 test_chr test_gets test_pool test_err test_regex test_obj test_sort test_write test_exec
    test_tok test_prs test_sym test_prs_top test_comp_obj test_link
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY test
)
