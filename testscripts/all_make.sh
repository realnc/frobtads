#! /bin/sh
# "Make" tests.

ret=0
cd "$T3_OUT"

# We don't perform the "hashes" test due to the line terminator differences
# between Unix/Windows.
for i in asi anon anonfunc anonobj anonvarg badnest bignum bignum2 findreplace \
         foreach funcparm htmlify ifnil inh_next isin join lclprop \
         listprop lookup lookup2 lookup3 lookupdef multidyn nested newprop \
         objloop opoverload propaddr propptr rexassert rexreplace setsc shr \
         spec2html spec2text split sprintf strcomp2 strbuf substr \
         unicode varmac vector vector2 vector3 testaddr2 testaddr3 testaddr4 \
         strtpl listminmax packstr packarr
do
    if $SCRIPTS/test_make.sh cp437 "$i" "$i"; then
        :
    else
        ret=1
    fi
done

# These need to output latin1 encoded characters.
for i in strcomp3
do
    if $SCRIPTS/test_make.sh latin1 "$i" "$i"; then
        :
    else
        ret=1
    fi
done

for i in catch save html addlist listpar arith
do
    if $SCRIPTS/test_make.sh cp437 -nodef "$i" "$i"; then
        :
    else
        ret=1
    fi
done

for i in extfunc objrep funcrep conflict
do
    if $SCRIPTS/test_make.sh cp437 -nodef "$i" ${i}1 ${i}2; then
        :
    else
        ret=1
    fi
done

if $SCRIPTS/test_make.sh cp437 -pre vocext vocext1 vocext2 reflect; then
    :
else
    ret=1
fi

for i in extern objmod
do
    if $SCRIPTS/test_make.sh cp437 -nodef "$i" ${i}1 ${i}2 ${i}3; then
        :
    else
        ret=1
    fi
done

if $SCRIPTS/test_make.sh cp437 -nodef gram2 tok gram2; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh cp437 -nodef rand rand; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh cp437 -debug stack stack reflect; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh cp437 -pre targprop targprop reflect; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh cp437 -pre clone clone reflect; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh cp437 -pre printexpr printexpr dynfunc; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh cp437 -pre dynctx dynctx dynfunc; then
    :
else
    ret=1
fi

# These tests require running preinit (test_make normally suppresses it)
for i in vec_pre symtab enumprop modtobj undef undef2 newembed newembederr \
         triplequote optargs optargs_err optargs_err2
do
    if $SCRIPTS/test_make.sh cp437 -pre "$i" "$i"; then
        :
    else
        ret=1
    fi
done

# Disable this for now; they can't work since they don't support building
# in different directory than where the sources reside.
#if $SCRIPTS/test_make.sh -pre multimethod_dynamic multimethod \
#                         multmethmultimethod_static -DMULTMETH_STATIC_INHERITED \
#                         multimethod multmeth
#then
#    :
#else
#    ret=1
#fi

if $SCRIPTS/test_make.sh cp437 -pre multimethod_dynamic2 multimethod multimethod2 \
                          multmeth
then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh cp437 -pre multimethod_static2 -DMULTMETH_STATIC_INHERITED \
                         multimethod multimethod2 multmeth
then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh cp437 -pre namedparam namedparam multmeth; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh cp437 -pre bifptr bifptr dynfunc; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh cp437 -pre setmethod setmethod dynfunc; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh cp437 -pre lclvars lclvars lclvars2 reflect dynfunc; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh cp437 -pre dynamicGrammar dynamicGrammar tok dynfunc gramprod
then
    :
else
    ret=1
fi

# ITER does a save/restore test
if $SCRIPTS/test_make.sh cp437 iter iter; then
    :
else
    ret=1
fi

if $SCRIPTS/test_restore.sh iter2 iter; then
    :
else
    ret=1
fi

exit $ret
