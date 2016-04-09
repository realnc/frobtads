main(args)
{
    "[default]\n";
    showString('a
     b');
    showString('a\n
     b');
    showString('a\n
     b\n
     c\n


     d
     e

     f
     g
     \n
     h');

    "\b[collapse]\n";
#pragma newline_spacing(collapse);
    showString('a
     b');

    "\b[delete]\n";
#pragma newline_spacing(delete);
    showString('a
     b');
    showString('a\n
     b');

    "\b[preserve]\n";
#pragma newline_spacing(preserve);
    showString('a
     b');
    showString('a\n
     b');
}

showString(s)
{
    "<<s>>: len=<<s.length()>>, chars=(<<s.toUnicode().join(',')>>)\n";
}
