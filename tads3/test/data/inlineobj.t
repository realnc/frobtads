/*
 *   in-line object definitions 
 */

#include <tads.h>
#include <dynfunc.h>

class Thing: object
    className = 'Thing'
;

class Item: Thing
    className = 'Item'
;

// a static object, for comparison
statobj: Thing
    name = 'statobj'
    val = 10*10*10
    desc = "This is object statobj with class <<className>>"
;

main()
{
    local i = 123;
    local j = 321;

    // show the static object
    test('statobj', statobj); "\n";

    // simple object with all constant property values
    local x = object {
        name = 'x';
        val = 987
        desc = "This is object x";
    };
    test('x', x); "\n";

    // try a constant expression for 'val', and a dstring embedding for 'desc'
    local y = object: Thing {
        name = 'y';
        val = 1+2+3+4
        desc = "This is object y with class <<className>>";
    };
    test('y', y); "\n";

    // add a dstring embedding that references 'i' in the enclosing scope;
    // this should create an anonymous method with a binding to 'i'
    local z = object: Thing {
        name = 'z';
        val = (11*11)*2
        desc = "This is object z, with class <<className>>; i++=<<i++>>";
    };

    // test twice - the second time confirms that we have a live link to 'i'
    // in the enclosing scope
    test('z', z); ", and now i=<<i>>\n";
    test('z', z); ", and now i=<<i>>\n";

    // 'val' evaluates 'j' on object creation but not on each property eval,
    // but now add an explicit method for 'desc'
    local w = object: Thing {
        name = 'w';
        val = static j++;
        desc { "This is object w, with class <<className>>; i++=<<i++>>"; }
    };

    // test twice to see the effects on 'i' and 'j'
    test('w', w); ", and now i=<<i>>, j=<<j>>\n";
    test('w', w); ", and now i=<<i>>, j=<<j>>\n";

    // change val to an implicit method that accesses 'j' from the local scope
    local w2 = object: Thing {
        name = 'w2';
        val = j++;
        desc = "This is object w2, with class <<className>>; i++=<<i++>>";
    };
    test('w2', w2); ", and now i=<<i>>, j=<<j>>\n";
    test('w2', w2); ", and now i=<<i>>, j=<<j>>\n";

    // change val to an explicit method that accesses 'j' from the local scope
    local w3 = object: Thing {
        name = 'w3';
        val { return j++; }
        desc = "This is object w2, with class <<className>>; i++=<<i++>>";
    };
    test('w3', w3); ", and now i=<<i>>, j=<<j>>\n";
    test('w3', w3); ", and now i=<<i>>, j=<<j>>\n";

    local v = createObj('v');
    test('v', v); ", and now i=<<i>>\n";
    test('v', v); ", and now i=<<i>>\n";

    local v2 = createObj('v2');
    test('v2', v2); ", and now i=<<i>>\n";
    test('v2', v2); ", and now i=<<i>>\n";

    local u = object {
        name = 'u';
        subobj {
            local ii = 456;
            return object: Item {
                val = static i++;
                valii = static ii++;

                name = 'u.subobj';
                desc = "This is u.subobj with class <<className>>,
                    val=<<val>>, valii=<<valii>>, ii++=<<ii++>>";
            };
        };
    };
    local usub = u.subobj;
    test('usub', usub); ", and now i=<<i>>\n";
    test('usub', usub); ", and now i=<<i>>\n";

    local t = object {
        name = 't';
        subobj: object {
            name = 'tsub';
            val = static i++;
            desc = "This is t.subobj with class <<className>>,
                val=<<val>>, i++=<<i++>>";
        };
    };
    local tsub = t.subobj;
    test('tsub', tsub); ", and now i=<<i>>\n";
    test('tsub', tsub); ", and now i=<<i>>\n";

    // test tsub again to make sure t.subobj is static
    tsub = t.subobj;
    test('tsub', tsub); ", and now i=<<i>>\n";

    local t2 = object {
        name = 't2';
        subobj: Thing {
            name = 't2sub';
            val = static i++;
            desc = "This is t2.subobj with class <<className>>,
                val=<<val>>, i++=<<i++>>";
        };
    };
    local t2sub = t2.subobj;
    test('t2sub', t2sub); ", and now i=<<i>>\n";
    test('t2sub', t2sub); ", and now i=<<i>>\n";

    local dyn = Compiler.eval('''
        object: Thing {
            name = 'dyn';
            val = 7*111;
            desc = "This is dyn with class \<<className>>!!!";
        }''');
    test('dyn', dyn);
}

createObj(name)
{
    local i = 7;
    return object: Item {
        name = name;
        val = static i*2;
        desc = "This is object <<name>>, with class <<className>>; i++=<<i++>>";
    };
}

test(name, obj)
{
    local typemap = ['nil', 'true', '3', '4', 'object',
                     'prop', 'int', 'sstring', 'dstring', 'list',
                     'code', 'funcptr', '13', 'native', 'enum',
                     'bifptr'];
    
    "<<name>>: name=<<obj.name>>,
    desc=<<obj.desc>> [type=<<typemap[obj.propType(&desc)]>>],
    val=<<obj.val>> [type=<<typemap[obj.propType(&val)]>>],
    className=<<obj.className>>";
}
