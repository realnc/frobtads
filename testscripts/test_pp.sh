#! /bin/sh

# Preprocessor tests
rm -f "$T3_OUT/$1.*"
mkdir -p $T3_OUT

ret=0
cd "$T3_OUT"
echo "Preprocessor test: $1"
$TESTPROGS/test_tok -I"$T3_DAT" -I"$T3_INCDIR" -P "$T3_DAT/$1.c" > "$T3_OUT/$1.log" 2> "$T3_OUT/$1.err"
cat "$T3_OUT/$1.err" >> "$T3_OUT/$1.log"
rm "$T3_OUT/$1.err"
if $SCRIPTS/test_diff.sh "$1"; then
    :
else
    ret=1
fi

exit $ret
