#! /bin/sh
# Pre-built Run Test: run a test program that's been separately compiled.

ret=0
cd "$T3_OUT"

echo "Pre-built Run test: $1"
tstrunlog=$1
shift

$TESTPROGS/test_exec -tz America/Los_Angeles -sd "$T3_OUT" -cs cp437 -norand "$T3_OUT/$1.t3" \
    $2 $3 $4 $5 $6 $7 $8 $9 > "$T3_OUT/$tstrunlog.log" 2>"$T3_OUT/$tstrunlog.err"

cat "$T3_OUT/$tstrunlog.err" >> "$T3_OUT/$tstrunlog.log"
rm "$T3_OUT/$tstrunlog.err"
if $SCRIPTS/test_diff.sh "$tstrunlog"; then
    :
else
    ret=1
fi

exit $ret
