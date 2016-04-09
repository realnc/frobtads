#include <tads.h>

main()
{
    local a = 1, b = 2;

    // this one is nested 9 deep, which is fine...
    "This the outer string. <<
      """This is the first inner string. <<
        'This is the second inner string. <<
          '''This is the third inner string. <<
            'Fourth. <<
              'Fifth. <<
                'Sixth. <<
                  'Seventh. <<
                    'Eighth. <<
                      'a=<<a>>, b=<<b>>'
                      >> End 8th.'
                    >> End 7th.'
                  >> End 6th.'
                >> End 5th.'
              >> End 4th.'
            >> That's it for the third.'''
          >> And for the second.'
        >> And the first."""
      >> And back to the outer.";
    "\b";

    // ...but this one goes to 10, which isn't
    "This the outer string. <<
      """This is the first inner string. <<
        'This is the second inner string. <<
          '''This is the third inner string. <<
            'Fourth. <<
              'Fifth. <<
                'Sixth. <<
                  'Seventh. <<
                    'Eighth. <<
                      'Ninth. <<
                        'a=<<a>>, b=<<b>>'
                        >> End 9th.'
                      >> End 8th.'
                    >> End 7th.'
                  >> End 6th.'
                >> End 5th.'
              >> End 4th.'
            >> That's it for the third.'''
          >> And for the second.'
        >> And the first."""
      >> And back to the outer.";
    "\b";
}
