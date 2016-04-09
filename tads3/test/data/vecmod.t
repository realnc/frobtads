#include <tads.h>

modify Vector
    testIndexing()
    {
        local val = self[1];
        self[1] = val.toUpper();
    }
;

main()
{
    local v = new Vector(['a', 'b', 'c']);
    v.testIndexing();
    "v = <<sayList(v)>>\n";
}

sayList(l)
{
    "[";
    for (local i in 1..l.length())
        "<<if i > 1>>, <<end>><<l[i]>>";
    "]";
}
