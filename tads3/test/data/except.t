#include <tads.h>
#include <filename.h>

main()
{
    test(f1);
    test(f2);
    test(f3);
    test(f4);
    test(f5);
}

test(f)
{
    try
    {
        f();
    }
    catch (Exception exc)
    {
        "Caught in test: <<exc.displayException()>>\n";
    }
    "\b";
}

f1()
{
    "f1...\n";
    FileName.fromUniversal('/').createDirectory();

    try
    {
        "Back from createDirectory (huh???)\n";
    }
    catch (Exception exc)
    {
        "Caught in f1: <<exc.displayException()>>\n";
    }
}

f2()
{
    "f2...\n";
    try
    {
        FileName.fromUniversal('/').createDirectory();
        "Back from createDirectory (huh???)\n";
    }
    catch (Exception exc)
    {
        "Caught in f2: <<exc.displayException()>>\n";
    }
}

f3()
{
    try
    {
        "f3...\n";
        FileName.fromUniversal('/').createDirectory();
        "Back from createDirectory (huh???)\n";
    }
    catch (Exception exc)
    {
        "Caught in f3: <<exc.displayException()>>\n";
    }
}

f4()
{
    try
    {
        "f4...\n";
        FileName.fromUniversal('/').createDirectory();
    }
    catch (Exception exc)
    {
        "Caught in f4: <<exc.displayException()>>\n";
    }
    "Back from createDirectory (okay)\n";
}

f5()
{
    try
    {
        "f5...\n";
    }
    catch (Exception exc)
    {
        "Caught in f5: <<exc.displayException()>>\n";
    }
    FileName.fromUniversal('/').createDirectory();
    "Back from createDirectory (huh???)\n";
}
