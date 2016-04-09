/*
 *   toString tests - there are some basic toString tests in builtin.t, but
 *   we test the function more extensively here 
 */

#include <tads.h>
#include <bignum.h>
#include <date.h>
#include <file.h>
#include <strbuf.h>

obj1: object
    prop1 = 'hello'
;

obj2: object
    prop2 = 'goodbye'
;

class A: object
    objToString = 'alpha&lt;<<name>>&gt;'
;

obj3: A
    name = 'three'
;

obj4: A
    name = 'four'
;

enum enum1, enum2;

#define test(arg) "toString(" ## #arg ## ") = <<toString(arg)>>\n"

main(args)
{
    test(&prop1);
    test(&prop2);
    test(&main);
    test(&_main);
    test(obj1);
    test(obj2);
    test(obj3);
    test(obj4);
    test(enum1);
    test(enum2);
    test(&toString);
    test(&toInteger);
    test(&tadsSay);
    test(new Date('9-oct-2011'));
    test(new FileName('foo/bar.txt'));
    test(new TimeZone('america/los_angeles'));
    test(new obj1());
    test([obj1, &prop1, enum1, 111, obj2, &prop2, enum2, 222.222]);
}

