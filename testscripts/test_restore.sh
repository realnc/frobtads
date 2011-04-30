#! /bin/sh
# Test restoring a saved state

echo "'Restore' test: $1"

$TESTPROGS/test_exec -cs cp437 "$T3_OUT/$2.t3" restore > "$T3_OUT/$1.log" 2>"$T3_OUT/$1.err"
cat "$T3_OUT/$1.err" >> "$T3_OUT/$1.log"
rm "$T3_OUT/$1.err"
$SCRIPTS/test_diff.sh "$1"
exit $?
