## This file contains Automake rules for the TADS 3 compiler.

## Include rules for the TADS 3 test suite if we're building the debug
## version.
##
if T3_DEBUG_BUILD
include $(srcdir)/t3compiler/Testsuite.am
endif

bin_PROGRAMS += t3make

AM_CPPFLAGS += -I$(srcdir)/t3compiler/src -I$(srcdir)/t3compiler/tads3 -I$(srcdir)/t3compiler/tads3/test

## Tell the compiler's base code to use the T3VM as target.
##
AM_CPPFLAGS += -DTC_TARGET_T3

## TADS 3 compiler headers.
##
T3CHEADERS = \
	t3compiler/tads3/t3_os.h \
	t3compiler/tads3/tct3ty.h \
	t3compiler/tads3/tct3drv.h \
	t3compiler/tads3/tct3int.h \
	t3compiler/tads3/tctarg.h \
	t3compiler/tads3/tct3base.h \
	t3compiler/tads3/tclibprs.h \
	t3compiler/tads3/tcunas.h \
	t3compiler/tads3/tcpndrv.h \
	t3compiler/tads3/tcpnint.h \
	t3compiler/tads3/tcpnbase.h \
	t3compiler/tads3/tccmdutl.h \
	t3compiler/tads3/tct3unas.h \
	t3compiler/tads3/tcerr.h \
	t3compiler/tads3/tcgen.h \
	t3compiler/tads3/tcprs.h \
	t3compiler/tads3/tcsrc.h \
	t3compiler/tads3/tctok.h \
	t3compiler/tads3/tcvsn.h \
	t3compiler/tads3/tcerrnum.h \
	t3compiler/tads3/core.h \
	t3compiler/tads3/tct3.h \
	t3compiler/tads3/tchostsi.h \
	t3compiler/tads3/tcglob.h \
	t3compiler/tads3/tchost.h \
	t3compiler/tads3/tcmain.h \
	t3compiler/tads3/tcmake.h \
	t3compiler/tads3/tctargty.h \
	t3compiler/tads3/rcmain.h \
	t3compiler/tads3/vmpreini.h

## TADS 3 compiler sources.
##
T3CSOURCES = \
	t3compiler/src/osportable3.cc \
	t3compiler/tads3/os_stdio.cpp \
	t3compiler/tads3/rcmain.cpp \
	t3compiler/tads3/std_dbg.cpp \
	t3compiler/tads3/tccmdutl.cpp \
	t3compiler/tads3/tcerr.cpp \
	t3compiler/tads3/tcerrmsg.cpp \
	t3compiler/tads3/tcgen.cpp \
	t3compiler/tads3/tcglob.cpp \
	t3compiler/tads3/tchostsi.cpp \
	t3compiler/tads3/tclibprs.cpp \
	t3compiler/tads3/tcmain.cpp \
	t3compiler/tads3/tcmakecl.cpp \
	t3compiler/tads3/tcmake.cpp \
	t3compiler/tads3/tcprs.cpp \
	t3compiler/tads3/tcprsimg.cpp \
	t3compiler/tads3/tcprsstm.cpp \
	t3compiler/tads3/tcsrc.cpp \
	t3compiler/tads3/tct3.cpp \
	t3compiler/tads3/tct3img.cpp \
	t3compiler/tads3/tct3stm.cpp \
	t3compiler/tads3/tct3unas.cpp \
	t3compiler/tads3/tctok.cpp \
	t3compiler/tads3/vmbifc.cpp \
	t3compiler/tads3/vmimgrb.cpp \
	t3compiler/tads3/vmpreini.cpp \
	t3compiler/tads3/vmwrtimg.cpp

## Compiler sources don't go into the main distribution, so we use
## 'nodist'.
##
nodist_t3make_SOURCES = $(COMMONSOURCES) $(T3CHEADERS) $(T3RCSOURCES) $(T3CSOURCES)

## Very simple targets for creating a package containing the compiler
## sources.
##
T3COMP_DISTFILES = \
	$(T3CHEADERS) \
	$(T3CSOURCES) \
	t3compiler/tads3/doc \
	t3compiler/tads3/include \
	t3compiler/tads3/lib \
	t3compiler/tads3/samples \
	t3compiler/tads3/test \
	t3compiler/testscripts \
	t3compiler/tads3/resnoexe.cpp \
	t3compiler/tads3/tcprs_d.cpp \
	t3compiler/tads3/tcprsnl.cpp \
	t3compiler/tads3/tct3nl.cpp \
	t3compiler/tads3/tct3_d.cpp

t3comp-dist:
	@cd "$(srcdir)" && $(AMTAR) chof - $(T3COMP_DISTFILES) | GZIP=$(GZIP_ENV) gzip -c >"@abs_builddir@/$(PACKAGE)-t3compiler-$(VERSION).tar.gz"

t3comp-dist-gzip: t3comp-dist

t3comp-dist-bzip2:
	@cd "$(srcdir)" && $(AMTAR) chof - $(T3COMP_DISTFILES) | bzip2 -9 -c >"@abs_builddir@/$(PACKAGE)-t3compiler-$(VERSION).tar.bz2"

## We cannot install whole directories at once; Automake requires us to
## list each and every file we wish to install.  This is normally a Good
## Thing for most projects.  But in this case, it's a brain damage, as
## the files we wish to install are not ours, but belong to the TADS 3
## base package.  Ah, well.
##
t3incdir = $(T3_INC_DIR)
t3libdir = $(T3_LIB_DIR)
t3libextensionsdir = $(T3_LIB_DIR)/extensions
t3libextensionstcommanddir = $(T3_LIB_DIR)/extensions/TCommand
t3libextensionstcommanddocdir = $(T3_LIB_DIR)/extensions/TCommand/doc
t3libadv3dir = $(T3_LIB_DIR)/adv3
t3libadv3enusdir = $(T3_LIB_DIR)/adv3/en_us
t3docdir = $(pkgdatadir)/tads3/doc

t3inc_DATA = \
	t3compiler/tads3/include/t3.h \
	t3compiler/tads3/include/strcomp.h \
	t3compiler/tads3/include/t3test.h \
	t3compiler/tads3/include/tok.h \
	t3compiler/tads3/include/charset.h \
	t3compiler/tads3/include/tadsgen.h \
	t3compiler/tads3/include/vector.h \
	t3compiler/tads3/include/systype.h \
	t3compiler/tads3/include/dict.h \
	t3compiler/tads3/include/file.h \
	t3compiler/tads3/include/tadsio.h \
	t3compiler/tads3/include/tadsiox.h \
	t3compiler/tads3/include/gramprod.h \
	t3compiler/tads3/include/bytearr.h \
	t3compiler/tads3/include/tads.h \
	t3compiler/tads3/include/reflect.h \
	t3compiler/tads3/include/lookup.h \
	t3compiler/tads3/include/bignum.h

t3lib_DATA = \
	t3compiler/tads3/lib/tok.t \
	t3compiler/tads3/lib/system.tl \
	t3compiler/tads3/lib/_main.t \
	t3compiler/tads3/lib/file.t \
	t3compiler/tads3/lib/gameinfo.t \
	t3compiler/tads3/lib/gramprod.t \
	t3compiler/tads3/lib/multmeth.t \
	t3compiler/tads3/lib/reflect.t

t3libextensions_DATA = \
	t3compiler/tads3/lib/extensions/combineReports.t \
	t3compiler/tads3/lib/extensions/cquotes.t \
	t3compiler/tads3/lib/extensions/custmsg.t \
	t3compiler/tads3/lib/extensions/customBanner.t \
	t3compiler/tads3/lib/extensions/CustomStatus.t \
	t3compiler/tads3/lib/extensions/newNames.t \
	t3compiler/tads3/lib/extensions/pathfind.t \
	t3compiler/tads3/lib/extensions/showTranscript.t \
	t3compiler/tads3/lib/extensions/SimpleAttachable.t \
	t3compiler/tads3/lib/extensions/smartAccompany.t \
	t3compiler/tads3/lib/extensions/subtime.t

t3libextensionstcommand_DATA = \
	t3compiler/tads3/lib/extensions/TCommand/GiveToAskFor.t \
	t3compiler/tads3/lib/extensions/TCommand/TCommand.t

t3libextensionstcommanddoc_DATA = \
	t3compiler/tads3/lib/extensions/TCommand/doc/cicon9.gif \
	t3compiler/tads3/lib/extensions/TCommand/doc/contpage.htm \
	t3compiler/tads3/lib/extensions/TCommand/doc/givetoaskfor.htm \
	t3compiler/tads3/lib/extensions/TCommand/doc/index.html \
	t3compiler/tads3/lib/extensions/TCommand/doc/introduction.htm \
	t3compiler/tads3/lib/extensions/TCommand/doc/tcommand.htm \
	t3compiler/tads3/lib/extensions/TCommand/doc/telltoaction.htm

t3libadv3_DATA = \
	t3compiler/tads3/lib/adv3/resolver.t \
	t3compiler/tads3/lib/adv3/travel.t \
	t3compiler/tads3/lib/adv3/lister.t \
	t3compiler/tads3/lib/adv3/extras.t \
	t3compiler/tads3/lib/adv3/report.t \
	t3compiler/tads3/lib/adv3/menusys.t \
	t3compiler/tads3/lib/adv3/pov.t \
	t3compiler/tads3/lib/adv3/score.t \
	t3compiler/tads3/lib/adv3/sense.t \
	t3compiler/tads3/lib/adv3/precond.t \
	t3compiler/tads3/lib/adv3/parser.t \
	t3compiler/tads3/lib/adv3/adv3.h \
	t3compiler/tads3/lib/adv3/thing.t \
	t3compiler/tads3/lib/adv3/tips.t \
	t3compiler/tads3/lib/adv3/exits.t \
	t3compiler/tads3/lib/adv3/modid.t \
	t3compiler/tads3/lib/adv3/exec.t \
	t3compiler/tads3/lib/adv3/verify.t \
	t3compiler/tads3/lib/adv3/misc.t \
	t3compiler/tads3/lib/adv3/events.t \
	t3compiler/tads3/lib/adv3/actor.t \
	t3compiler/tads3/lib/adv3/adv3.tl \
	t3compiler/tads3/lib/adv3/output.t \
	t3compiler/tads3/lib/adv3/action.t \
	t3compiler/tads3/lib/adv3/disambig.t \
	t3compiler/tads3/lib/adv3/objects.t \
	t3compiler/tads3/lib/adv3/hintsys.t \
	t3compiler/tads3/lib/adv3/status.t \
	t3compiler/tads3/lib/adv3/to_do.txt \
	t3compiler/tads3/lib/adv3/actions.t \
	t3compiler/tads3/lib/adv3/banner.t \
	t3compiler/tads3/lib/adv3/footnote.t \
	t3compiler/tads3/lib/adv3/input.t \
	t3compiler/tads3/lib/adv3/numbers.t \
	t3compiler/tads3/lib/adv3/settings.t \
	t3compiler/tads3/lib/adv3/changes.htm

t3libadv3enus_DATA = \
	t3compiler/tads3/lib/adv3/en_us/msg_neu.t \
	t3compiler/tads3/lib/adv3/en_us/en_us.h \
	t3compiler/tads3/lib/adv3/en_us/en_us.t \
	t3compiler/tads3/lib/adv3/en_us/en_us.tl \
	t3compiler/tads3/lib/adv3/en_us/instruct.t

t3doc_DATA = \
	t3compiler/tads3/doc/getacro.gif \
	t3compiler/tads3/doc/index.htm \
	t3compiler/tads3/doc/nodoc.htm \
	t3compiler/tads3/doc/nolibref.htm \
	t3compiler/tads3/doc/t3changes.htm \
	t3compiler/tads3/doc/title2.gif

## Target to build the TADS 3 sample game.  We'll always rebuild it, no
## matter if it already exists or not.
##
sample: t3make
	test -z "@abs_builddir@/samples/obj" || $(mkinstalldirs) "@abs_builddir@/samples/obj"
	rm -f "@abs_builddir@/samples/sample.t3m"
	cp "$(srcdir)/t3compiler/tads3/samples/sample.t3m" "@abs_builddir@/samples/sample.t3m"
	cd "@abs_builddir@/samples" && "@abs_builddir@/t3make" -f sample.t3m -a -FL "@abs_srcdir@/t3compiler/tads3/lib" -FI "@abs_srcdir@/t3compiler/tads3/include" -I "@abs_srcdir@/t3compiler/tads3/samples" -Fs "@abs_srcdir@/t3compiler/tads3/samples" -Fy "@abs_builddir@/samples/obj" -Fo "@abs_builddir@/samples/obj"

sample-clean:
	rm -f "@abs_builddir@/samples/sample.t3m" "@abs_builddir@/samples/sample.t3" "@abs_builddir@/samples/gameinfo.txt"
	rm -rf "@abs_builddir@/samples/obj"
