## This file contains Automake rules for the TADS 2 compiler.

bin_PROGRAMS += tadsc

AM_CPPFLAGS += -I$(srcdir)/t2compiler/src -I$(srcdir)/t2compiler/tads2

## TADS 2 compiler headers.
##
T2CHEADERS = \
	t2compiler/tads2/tcd.h \
	t2compiler/tads2/tcg.h

## TADS 2 compiler sources.
##
T2CSOURCES = \
	t2compiler/tads2/tcd.c \
	t2compiler/tads2/tcgdum.c \
	t2compiler/tads2/linf.c \
	t2compiler/tads2/tok.c \
	t2compiler/tads2/tokth.c \
	t2compiler/tads2/fiowrt.c \
	t2compiler/tads2/objcomp.c \
	t2compiler/tads2/sup.c \
	t2compiler/tads2/voccomp.c \
	t2compiler/tads2/prscomp.c \
	t2compiler/tads2/prs.c \
	t2compiler/tads2/emt.c \
	t2compiler/tads2/dbg.c

## Compiler sources don't go into the main distribution, so we use
## 'nodist'.
##
nodist_tadsc_SOURCES = t2compiler/src/main.c $(COMMONSOURCES) $(T2RCHEADERS) $(T2CHEADERS) $(T2RCSOURCES) $(T2CSOURCES)

## Installs the TADS 2 development library.
##
t2datadir = $(pkgdatadir)/tads2
t2data_DATA = t2compiler/tads2/adv.t t2compiler/tads2/std.t t2compiler/tads2/gameinfo.t

## Very simple targets for creating a package containing the compiler
## sources.
##
T2COMP_DISTFILES = t2compiler/src/main.c $(T2CHEADERS) $(T2CSOURCES) $(t2data_DATA)

t2comp-dist:
	@cd "$(srcdir)" && $(AMTAR) chof - $(T2COMP_DISTFILES) | GZIP=$(GZIP_ENV) gzip -c >"@abs_builddir@/$(PACKAGE)-t2compiler-$(VERSION).tar.gz"

t2comp-dist-gzip: t2comp-dist

t2comp-dist-bzip2:
	@cd "$(srcdir)" && $(AMTAR) chof - $(T2COMP_DISTFILES) | bzip2 -9 -c >"@abs_builddir@/$(PACKAGE)-t2compiler-$(VERSION).tar.bz2"
