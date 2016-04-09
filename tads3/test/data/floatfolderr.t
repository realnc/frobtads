/*
 *   Tests of some constant expressions that aren't allowed to include
 *   floating point values 
 */

main()
{
    local a1 = 1.0 << 1;    // invalid
    local a2 = 1 >> 1.0;    // invalid
    local a3 = 1.0 >> 1.0;  // invalid
    local b1 = 1 << 1;      // ok
    local b2 = 1 >> 2;      // ok
    
    local c1 = 1.0 & 1;     // invalid
    local c2 = 1 & 1.0;     // invalid
    local c3 = 1.0 & 1.0;   // invalid
    local c4 = 1 & 1;       // ok

    local d1 = 1 | 1.0;     // invalid
    local d2 = 1.0 | 1;     // invalid
    local d3 = 1.0 | 1.0;   // invalid
    local d4 = 1 | 1;       // ok
    
    local e1 = 1 ^ 1.0;     // invalid
    local e2 = 1.0 ^ 1;     // invalid
    local e3 = 1.0 ^ 1.0;   // invalid
    local e4 = 1 ^ 1;       // ok
}
