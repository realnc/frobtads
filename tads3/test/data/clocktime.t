#include <tads.h>
#include <date.h>


main()
{
    local d = new Date(2014, 6, 23, 15, 30, 45, 678);
    local t = d.getClockTime();
    "<<d>>.getClockTime = <<t.join(', ')>>\n";
}
