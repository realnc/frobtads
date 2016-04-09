#include <tads.h>

main()
{
    local x = 1.2345, y = 100;

    "x=<<x>>, x(%.2f)=<<%.2f x>>, x*3(%.2f)=<<%.2f x*3>>\n";
    "x in currency is $<<%_*7.2f x>>\n";
    "y in hex is <<%x y>>, octal <<%o y>>, decimal <<y>>\n";
}
