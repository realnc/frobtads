#! /bin/sh

# Execution tests
ret=0
cd "$T3_OUT"

echo "Execution test: $1"
$TESTPROGS/test_prs_top -I"$T3_INCDIR" "$T3_DAT/$1.t" "$T3_OUT/$1.t3" > "$T3_OUT/$1.log" 2>"$T3_OUT/$1.err"
cat "$T3_OUT/$1.err" >> "$T3_OUT/$1.log"
$TESTPROGS/test_exec -cs $2 -norand "$T3_OUT/$1.t3" >> "$T3_OUT/$1.log" 2>"$T3_OUT/$1.err"
cat "$T3_OUT/$1.err" >> "$T3_OUT/$1.log"
rm "$T3_OUT/$1.err"
if $SCRIPTS/test_diff.sh "$1"; then
    :
else
    ret=1
fi

exit $ret
