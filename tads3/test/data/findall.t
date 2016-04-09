#include <tads.h>

main()
{
    test('one two three four five', R'<alpha>+');
    test('one 1 two 22 three 333 four 4444 five 55555', R'%d+');
    test('alfa1 bravo22 charlie333 delta444', R'(<alpha>+)(%d+)',
         {m: '<<m>> {<<argcount>>}'});
    test('alfa1 bravo22 charlie333 delta444', R'(<alpha>+)(%d+)',
         {m, i: '<<m>>, <<i>> {<<argcount>>}'});
    test('alfa1 bravo22 charlie333 delta444', R'(<alpha>+)(%d+)',
         {m, i, g1: '<<m>>, <<i>>, <<g1>> {<<argcount>>}'});
    test('alfa1 bravo22 charlie333 delta444', R'(<alpha>+)(%d+)',
         {m, i, g1, g2: '<<m>>, <<i>>, <<g1>>, <<g2>> {<<argcount>>}'});
    test('alfa1 bravo22 charlie333 delta444', R'(<alpha>+)(%d+)',
         {m, i, g1, g2, g3: '<<m>>, <<i>>, <<g1>>, <<g2>>, <<g3>>
             {<<argcount>>}'});
    test('alfa1 bravo22 charlie333 delta444', R'(<alpha>+)(%d+)',
         {m, i, [groups]: '<<m>>, <<i>>, [<<groups>>] {<<argcount>>}'});
    test('alfa1 bravo22 charlie333 delta444', R'(<alpha>+)(%d+)',
         {m, i, g1, [groups]: '<<m>>, <<i>>, <<g1>>, [<<groups>>]
             {<<argcount>>}'});
    test('alfa1 bravo22 charlie333 delta444', R'(<alpha>+)(%d+)',
         {m, i, g1, g2, [groups]: '<<m>>, <<i>>, <<g1>>, <<g2>>, [<<groups>>]
             {<<argcount>>}'});
    test('alfa1 bravo22 charlie333 delta444', R'(<alpha>+)(%d+)',
         {m, i, g1, g2, g3, [groups]: '<<m>>, <<i>>, <<g1>>, <<g2>>, <<g3>>,
             [<<groups>>] {<<argcount>>}'});
}

test(str, pat, func?)
{
    local patstr = (pat.ofKind(RexPattern)
                    ? 'R\'<<toString(pat)>>\'' : '\'<<pat>>\'');
    if (func == nil)
        "<<str.htmlify()>>.findAll(<<patstr.htmlify()>>) = [<<
          str.findAll(pat)>>]\n";
    else
        "<<str.htmlify()>>.findAll(<<patstr.htmlify()>>, func) = [<<
          str.findAll(pat, func)>>]\n";
}

