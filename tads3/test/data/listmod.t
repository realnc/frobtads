#include <tads.h>

modify List
    testIndexing()
    {
        local val = self[1];
        self[1] = val.toUpper();
    }
;

main()
{
    local l = ['a', 'b', 'c'];
    l.testIndexing();
    "l = <<sayList(l)>>\n";
}

sayList(l)
{
    "[";
    for (local i in 1..l.length())
        "<<if i > 1>>, <<end>><<l[i]>>";
    "]";
}
