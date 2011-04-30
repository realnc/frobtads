#! /bin/sh
# "Make" tests.

ret=0
cd "$T3_OUT"

for i in asi anon anonfunc anonobj anonvarg badnest bignum bignum2 findreplace \
         foreach funcparm hashes htmlify ifnil inh_next isin join lclprop listprop \
         lookup lookup2 lookup3 lookupdef multidyn nested newprop objloop \
         opoverload propaddr propptr rexassert rexreplace setsc shr spec2html \
         spec2text split sprintf strcomp2 strcomp3 strbuf substr unicode varmac vector \
         vector2 vector3 testaddr2 testaddr3 testaddr4 strtpl listminmax
do
    if $SCRIPTS/test_make.sh "$i" "$i"; then
        :
    else
        ret=1
    fi
done

for i in catch save html addlist listpar arith
do
    if $SCRIPTS/test_make.sh -nodef "$i" "$i"; then
        :
    else
        ret=1
    fi
done

for i in extfunc objrep funcrep conflict
do
    if $SCRIPTS/test_make.sh -nodef "$i" ${i}1 ${i}2; then
        :
    else
        ret=1
    fi
done

if $SCRIPTS/test_make.sh -pre vocext vocext1 vocext2 reflect; then
    :
else
    ret=1
fi

for i in extern objmod
do
    if $SCRIPTS/test_make.sh -nodef "$i" ${i}1 ${i}2 ${i}3; then
        :
    else
        ret=1
    fi
done

if $SCRIPTS/test_make.sh -nodef gram2 tok gram2; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh -nodef rand rand; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh -debug stack stack reflect; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh -pre targprop targprop reflect; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh -pre clone clone reflect; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh -pre printexpr printexpr dynfunc; then
    :
else
    ret=1
fi

if $SCRIPTS/test_make.sh -pre dynctx dynctx dynfunc; then
    :
else
    ret=1
fi

# These tests require running preinit (testmake normally suppresses it)
for i in vec_pre symtab enumprop modtobj undef undef2
do
    if $SCRIPTS/test_make.sh "$i" -pre "$i"; then
        :
    else
        ret=1
    fi
done

# ITER does a save/restore test
if $SCRIPTS/test_make.sh iter iter; then
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
