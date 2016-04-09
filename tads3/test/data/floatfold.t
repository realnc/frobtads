/*
 *   Floating point (BigNumber) constant folding tests.  The compiler
 *   didn't do any constant folding prior to 3.1.1; it now folds
 *   float+float, int+float, and float+int constant expressions for the
 *   basic arithmetic operators (+ - * / %) and comparisons (== != < > <=
 *   >=).  This test checks to make sure that foldable constant expressions
 *   are in fact folded by testing property types (an unfolded expression
 *   will have type "code", whereas a folded value will be a BigNumber
 *   object).
 */

#include <tads.h>
#include <bignum.h>
#include <reflect.h>

obj: object
    // addition
    prop1 = 1 + 0.1
    prop2 = 0.2 + 2
    prop3 = 3.0 + .3
    prop4 = 3 + 1

    // subtraction
    prop5 = 6 - 0.5
    prop6 = 0.4 - 7
    prop7 = 8.0 - 0.3
    prop8 = 11 - 3

    // negation
    prop9 = -(8.0 + 1)
    prop10 = -(11 - 1)

    // multiplication
    prop11 = 2 * 5.5
    prop12 = 2.4 * 5
    prop13 = 2.5 * 5.2
    prop14 = 2 * 7

    // division
    prop15 = 108 / 7.2
    prop16 = 32.0 / 2
    prop17 = 42.5 / 2.5
    prop18 = 54 / 3

    // remainder
    prop19 = 88 % 23.0
    prop20 = 223.0 % 29
    prop21 = 238.0 % 31.0
    prop22 = 133 % 37

    // comparisons
    prop23 = (1.000 == 1)
    prop24 = (1.001 == 1)
    prop25 = (1 == 1.000)
    prop26 = (1.000 == 1)
    prop27 = (1.000 == 1.00)
    prop28 = (1.000 == 1.01)

    prop29 = (1 > 1.000)
    prop30 = (1 > 1.001)
    prop31 = (1 > 0.999)

    prop32 = (1.000 > 1)
    prop33 = (1.001 > 1)
    prop34 = (0.999 > 1)

    prop35 = (1.000 > 1.00)
    prop36 = (1.000 > 1.001)
    prop37 = (1.000 > 0.999)

    prop38 = (1 < 1.000)
    prop39 = (1 < 1.001)
    prop40 = (1 < 0.999)

    prop41 = (1.000 < 1)
    prop42 = (1.001 < 1)
    prop43 = (0.999 < 1)

    prop44 = (1.000 < 1.00)
    prop45 = (1.000 < 1.001)
    prop46 = (1.000 < 0.999)

    prop47 = (1 >= 1.000)
    prop48 = (1 >= 1.001)
    prop49 = (1 >= 0.999)

    prop50 = (1.000 >= 1)
    prop51 = (1.001 >= 1)
    prop52 = (0.999 >= 1)

    prop53 = (1.000 >= 1.00)
    prop54 = (1.000 >= 1.001)
    prop55 = (1.000 >= 0.999)

    prop56 = (1 <= 1.000)
    prop57 = (1 <= 1.001)
    prop58 = (1 <= 0.999)

    prop59 = (1.000 <= 1)
    prop60 = (1.001 <= 1)
    prop61 = (0.999 <= 1)

    prop62 = (1.000 <= 1.00)
    prop63 = (1.000 <= 1.001)
    prop64 = (1.000 <= 0.999)

    prop65 = (1.000 != 1)
    prop66 = (1.001 != 1)
    prop67 = (1 != 1.000)
    prop68 = (1.000 != 1)
    prop69 = (1.000 != 1.00)
    prop70 = (1.000 != 1.01)

    // logical NOT
    prop71 = !1.0
    prop72 = !0.0

    // local AND
    prop73 = 0.0 && 0.0
    prop74 = 0.0 && 1.0
    prop75 = 1.0 && 0.0
    prop76 = 1.0 && 1.0
    prop77 = 0 && 0.0
    prop78 = 0 && 1.0
    prop79 = 1 && 0.0
    prop80 = 1 && 1.0
    prop81 = 0.0 && 0
    prop82 = 0.0 && 1
    prop83 = 1.0 && 0
    prop84 = 1.0 && 1

    // local OR
    prop85 = 0.0 || 0.0
    prop86 = 0.0 || 1.0
    prop87 = 1.0 || 0.0
    prop88 = 1.0 || 1.0
    prop89 = 0 || 0.0
    prop90 = 0 || 1.0
    prop91 = 1 || 0.0
    prop92 = 1 || 1.0
    prop93 = 0.0 || 0
    prop94 = 0.0 || 1
    prop95 = 1.0 || 0
    prop96 = 1.0 || 1

    // overflows
    prop97 = 2147483646
    prop98 = 2147483646 + 1
    prop99 = 2147483647
    prop100 = 2147483647 + 1
    prop101 = -2147483647
    prop102 = -2147483647 - 1
    prop103 = -2147483648
    prop104 = -2147483648 - 1
    prop105 = 2147483648
    prop106 = -2147483649
    prop107 = 3000000000
    prop108 = -3000000000
    prop109 = 0x100000000
    prop110 = 040000000000
    prop111 = 0x2540BE400 + 111
    prop112 = 112 + 0x2540BE400
    prop113 = 0x2540BE400 + 0x2540BE471
    prop114 = 0xffffffff
    prop115 = 037777777777
    prop116 = -(-2147483647)
    prop117 = -(-2147483648)
    prop118 = -(-2147483649)
    prop119 = -(-2147483647 - 1)
    prop120 = -(-(-2147483647 - 1))
    prop121 = (0x7fffffff + 1)
    prop122 = 0x80000000
    prop123 = 0x80000000 - 1
;

main()
{
    for (local i = 1 ; ; ++i)
    {
        local propname = 'prop<<i>>';
        local prop = reflectionServices.symtab_[propname];
        if (prop == nil)
            break;

        "<<propname>> = ";
        local val = obj.(prop);
        switch (obj.propType(prop))
        {
        case TypeObject:
            "[<<reflectionServices.valToSymbol(
                val.getSuperclassList()[1])>>] = <<val>>\n";
            break;

        case TypeInt:
            "[int] <<val>>\n";
            break;

        case TypeCode:
            "[code] <<val>>\n";
            break;

        case TypeTrue:
            "true\n";
            break;

        case TypeNil:
            "nil\n";
            break;

        default:
            "[other] <<val>>\n";
            break;
        }
    }
}

