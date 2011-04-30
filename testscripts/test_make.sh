#! /bin/sh

# Make and execute tests

case "$1" in
    -nodef)
        shift
        $t3make -test -I "$T3_DAT" -a -nodef -nobanner -nopre -Fs "$T3_DAT" -Fo "$T3_OUT" -Fy "$T3_OUT" -o "$T3_OUT/$1.t3" $2 $3 $4 $5 $6 $7 $8 $9 > "$T3_OUT/$1.log" 2>"$T3_OUT/$1.err"
        ;;
    -debug)
        shift
        $t3make -test -d -I "$T3_DAT" -a -nobanner -nopre -Fs "$T3_DAT" -Fo "$T3_OUT" -Fy "$T3_OUT" -o "$T3_OUT/$1.t3" $2 $3 $4 $5 $6 $7 $8 $9 > "$T3_OUT/$1.log" 2>"$T3_OUT/$1.err"
        ;;
    -pre)
        shift
        $t3make -test -I "$T3_DAT" -a -nobanner -Fs "$T3_DAT" -Fo "$T3_OUT" -Fy "$T3_OUT" -o "$T3_OUT/$1.t3" $2 $3 $4 $5 $6 $7 $8 $9 > "$T3_OUT/$1.log" 2>"$T3_OUT/$1.err"
        ;;
    *)
        $t3make -test -I "$T3_DAT" -a -nobanner -nopre -Fs "$T3_DAT" -Fo "$T3_OUT" -Fy "$T3_OUT" -o "$T3_OUT/$1.t3" $2 $3 $4 $5 $6 $7 $8 $9 > "$T3_OUT/$1.log" 2>"$T3_OUT/$1.err"
        ;;
esac
cat "$T3_OUT/$1.err" >> "$T3_OUT/$1.log"

echo "'Make' test: $1"

$TESTPROGS/test_exec -cs cp437 "$T3_OUT/$1.t3" >> "$T3_OUT/$1.log" 2>"$T3_OUT/$1.err"
cat "$T3_OUT/$1.err" >> "$T3_OUT/$1.log"
rm "$T3_OUT/$1.err"
$SCRIPTS/test_diff.sh "$1"
exit $?
