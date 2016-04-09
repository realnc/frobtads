#include <tads.h>

main()
{
    local a = 1, b = 2;
    "This the outer string. <<
      """This is the first inner string. <<
        'This is the second inner string. <<
          '''This is the third inner string. <<
            'a=<<a>>, b=<<b>>'
            >> That's it for the third.'''
          >> And for the second.'
        >> And the first."""
      >> And back to the outer.";
    "\b";

    "Another test, this time with all dquote strings. <<
      "First inner. <<
        "Second inner. <<
          "Third inner: a=<<a>>, b=<<b>>."
          >> End of second."
        >> End of first."
      >> End of outer.";
    "\b";

    local desc = function(open, lit) {
        "The regrigerator door is <<
          open ? "open<<lit ? " and the light inside is on" : ""
         >>" : "closed">>.\n";
    };
    desc(nil, nil);
    desc(nil, true);
    desc(true, nil);
    desc(true, true);
}

