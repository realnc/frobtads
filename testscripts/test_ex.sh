#! /bin/sh

# Execution tests
ret=0
cd "$T3_OUT"

for i in basic finally dstr fnredef builtin undo gotofin
do
    echo "Execution test: $i"
    $TESTPROGS/test_prs_top -I"$T3_INCDIR" "$T3_DAT/$i.t" "$T3_OUT/$i.t3" > "$T3_OUT/$i.log" 2>"$T3_OUT/$i.err"
    cat "$T3_OUT/$i.err" >> "$T3_OUT/$i.log"
    $TESTPROGS/test_exec -cs cp437 -norand "$T3_OUT/$i.t3" >> "$T3_OUT/$i.log" 2>"$T3_OUT/$i.err"
    cat "$T3_OUT/$i.err" >> "$T3_OUT/$i.log"
    rm "$T3_OUT/$i.err"
    if $SCRIPTS/test_diff.sh "$i"; then
        :
    else
        ret=1
    fi
done

exit $ret
