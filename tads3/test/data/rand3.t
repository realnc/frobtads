#include <tads.h>

main()
{
    // isaac fixed seeding
    
    randomize(RNG_ISAAC, 'testing');
    local r1 = Vector.generate({: rand() }, 100);

    randomize(RNG_ISAAC, 'testing2');
    local r2 = Vector.generate({: rand() }, 100);

    local state_i2 = randomize(nil);
    local save_i2 = Vector.generate({: rand() }, 100);

    randomize(RNG_ISAAC, 'testing');
    local r3 = Vector.generate({: rand() }, 100);

    randomize(RNG_ISAAC, 'testing');
    local r4 = Vector.generate({: rand() }, 100);

    randomize(RNG_ISAAC, 'testing2');
    local r5 = Vector.generate({: rand() }, 100);

    "r1 (isaac,'testing') = <<r1.join(' ')>>\b";
    "r2 (isaac,'testing2) = <<r2.join(' ')>>\b";
    "save_i2 = <<save_i2.join(' ')>>\b";
    "r3 == r1? <<r3 == r1 ? 'yes' : 'no'>>\n";
    "r4 == r1? <<r4 == r1 ? 'yes' : 'no'>>\n";
    "r5 == r2? <<r5 == r2 ? 'yes' : 'no'>>\n";
    "\b";

    // LCG fixed seeding

    randomize(RNG_LCG, 123);
    local state_l1 = randomize(nil);
    r1 = Vector.generate({: rand() }, 100);

    randomize(RNG_LCG, 'test');
    r2 = Vector.generate({: rand() }, 100);

    randomize(RNG_LCG, 'test');
    r3 = Vector.generate({: rand() }, 100);

    randomize(RNG_LCG, 123);
    r4 = Vector.generate({: rand() }, 100);

    "r1 (lcg,123) = <<r1.join(' ')>>\b";
    "r2 (lcg,'test') = <<r2.join(' ')>>\b";
    "r3 == r2 ? <<r3 == r2 ? 'yes' : 'no'>>\n";
    "r4 == r1 ? <<r4 == r1 ? 'yes' : 'no'>>\n";
    "\b";

    // back to ISAAC to test switching
    randomize(RNG_ISAAC, 'testing2');
    r4 = Vector.generate({: rand() }, 100);
    "back to isaac: r4 == r5 ? <<r4 == r5 ? 'yes' : 'no'>>\n";
    "\b";

    // restore the LCG(123) state
    randomize(state_l1);
    r4 = Vector.generate({: rand() }, 100);
    "restoring LCG(123) state: r4 == r1 ? <<r4 == r1 ? 'yes' : 'no'>>\n";
    "\b";

    // restore the ISAAC('testing2') state
    randomize(state_i2);
    r4 = Vector.generate({: rand() }, 100);
    "restoring state_i2[ISAAC,'testing2'] state: r4 == save_i2 ? <<
      r4 == save_i2 ? 'yes' : 'no'>>\n";
    "\b";

    // try MT19937
    randomize(RNG_MT19937, 'initial mt seed');
    r1 = Vector.generate({: rand() }, 100);
    "r1 (mt,'initial mt seed') = <<r1.join(' ')>>\b";

    local state_m1 = randomize(nil);
    r2 = Vector.generate({: rand(100)}, 100);
    "r2 (continuing with mt) = <<r2.join(' ')>>\b";

    randomize(RNG_MT19937, 'another mt seed');
    r3 = Vector.generate({: rand(100) }, 100);
    "r3 (mt,'another mt seed') = <<r3.join(' ')>>\b";

    randomize(RNG_MT19937, 1234);
    r4 = Vector.generate({: rand(100) }, 100);
    "r4 (mt,1234) = <<r4.join(' ')>>\b";

    randomize(state_m1);
    r5 = Vector.generate({: rand(100)}, 100);
    "restoring mt: r5 == r2 ? <<r5 == r2 ? 'yes' : 'no'>>\b";
}

