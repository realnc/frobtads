/*
 *   Constant regular expression value tests 
 */

#include <tads.h>

obj1: object
    rex1 = R"%d+"
    rex2 = R'%w+'
;

main()
{
    test(R'[0-4]+');
    test(R"[5-9]+");

    local r = R'[a-m]+';
    test(r);

    test(obj1.rex1);
    test(obj1.rex2);

    testTitle('this is a test string');
}

test(re)
{
    local str = '... abcdefghijklmnopqrstuvwxyz 0123456789 ...';
    local repl = str.findReplace(re, '[[[%*]]]');
    "<<re>> -&gt; <<repl>>\n";
}

testTitle(str)
{
    "<<str>> &gt;&gt;titleCase-&gt; <<
      str.findReplace(R'%<%w', {x: x.toTitleCase()})>>\n";
}
