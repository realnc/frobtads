#include <tads.h>

WaitAction: object
    execAction() {
        local test = 1;
        if (testObject.propDefined(&operator []=))
        {
            "[]= is defined\n";
            testObject[1] = 2;
        }
        if (testObject.propDefined(&operator []))
        {
            "[] is defined\n";
            test = testObject['abc'];
        }
        "<<test>>";
    }
;

testObject: object
    operator[](b) {
        return b;
    }

    operator[]=(b,c) {
        return self;
    }
;

main()
{
    WaitAction.execAction();
}
