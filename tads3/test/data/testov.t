#include <tads.h>
#include <bignum.h>

main()
{
    testAdd(10000, 10000);
    testAdd(100000, 100000);
    testAdd(0x7ffffffe, 1);
    testAdd(0x7ffffffe, 2);
    testAdd(0x7ffffffe, 3);
    testAdd(0x7ffffffe, 4);
    testAdd(0x7fffffff, 0);
    testAdd(0x7fffffff, 0x7fffffff);
    testAdd(-0x7fffffff, -0x7fffffff);
    testAdd(0x7fffffff, -1);
    testAdd(-0x7fffffff, 1);
    testAdd(-0x7fffffff, -1);
    testAdd(-0x7fffffff, -2);
    "\b";

    testInc(0x7ffffffd);
    testInc(0x7ffffffe);
    testInc(0x7fffffff);
    "\b";

    testSub(10000, 10000);
    testSub(10000, -10000);
    testSub(100000, 100000);
    testSub(100000, -100000);
    testSub(0x7ffffffe, 1);
    testSub(0x7ffffffe, 0);
    testSub(0x7ffffffe, -1);
    testSub(0x7ffffffe, -2);
    testSub(0x7ffffffe, -3);
    testSub(-0x7fffffff, -1);
    testSub(-0x7fffffff, 0);
    testSub(-0x7fffffff, 1);
    testSub(-0x7fffffff, 2);
    testSub(-0x7fffffff, 3);
    "\b";

    testDec(-2147483646);
    testDec(-2147483647);
    testDec(-2147483648);
    "\b";

    testMul(1000, 1000);
    testMul(1000, -1000);
    testMul(1000000, 1000);
    testMul(1000000, 2000);
    testMul(1000000, 3000);
    testMul(1000000, 4000);
    testMul(1000000, 5000);
    testMul(-1000000, 1000);
    testMul(-1000000, 2000);
    testMul(-1000000, 3000);
    testMul(-1000000, 4000);
    testMul(-1000000, 5000);
    testMul(1000000, -1000);
    testMul(1000000, -2000);
    testMul(1000000, -3000);
    testMul(1000000, -4000);
    testMul(1000000, -5000);
    testMul(0x7fffffff, 0x7fffffff);
    "\b";

    testDiv(1000, 1);
    testDiv(1000, -1);
    testDiv(1000, 2);
    testDiv(-2147483648, 1);
    testDiv(-2147483648, -2);
    testDiv(-2147483648, -1);
    "\b";

    testNeg(1000);
    testNeg(1000000);
    testNeg(1000000000);
    testNeg(2000000000);
    testNeg(-2000000000);
    testNeg(2147483647);
    testNeg(-2147483647);
    testNeg(-2147483648);
}

testAdd(a, b)
{
    local sum = a + b;
    "<<a>> + <<b>> = <<sum>><<if dataType(sum) == TypeObject
      >> *promoted*<<end>>\n";
}

testInc(a)
{
    local x = a;
    "++(<<a>>) = <<++x>><<if dataType(x) == TypeObject>> *promoted*<<end>>\n";

    x = a;
    x += 1;
    "<<a>> += 1 -&gt; <<x>><<
      if dataType(x) == TypeObject>> *promoted*<<end>>\n";
}

testSub(a, b)
{
    local diff = a - b;
    "<<a>> - <<b>> = <<diff>><<if dataType(diff) == TypeObject
      >> *promoted*<<end>>\n";
}

testDec(a)
{
    local x = a;
    "--(<<a>>) = <<--x>><<if dataType(x) == TypeObject>> *promoted*<<end>>\n";

    x = a;
    x -= 1;
    "<<a>> -= 1 -&gt; <<x>><<
      if dataType(x) == TypeObject>> *promoted*<<end>>\n";
}

testMul(a, b)
{
    local prod = a * b;
    "<<a>> * <<b>> = <<prod>><<if dataType(prod) == TypeObject
      >> *promoted*<<end>>\n";
}

testDiv(a, b)
{
    local quo = a / b;
    "<<a>> / <<b>> = <<quo>><<if dataType(quo) == TypeObject
      >> *promoted*<<end>>\n";
}

testNeg(a)
{
    local neg = -a;
    "-<<if a < 0>>(<<a>>)<<else>><<a>><<end>> = <<neg>><<
      if dataType(neg) == TypeObject>> *promoted*<<end>>\n";
}

