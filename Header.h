#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

uint64_t mult32_mod_pS(uint64_t a, uint64_t p)
{
    a = a % p;
    uint64_t a1 = a >> 32;
    uint64_t b1 = 0xffffffff & a;
    uint64_t residue = ((b1 << 32) % p) + a1 * 50;
    return residue % p;
}

uint64_t mult64_mod_pS(uint64_t a, uint64_t p)
{
    a = a % p;
    uint64_t a1 = a >> 32;
    uint64_t b1 = 0xffffffff & a;
    uint64_t residue = mult32_mod_pS(a1 * 50, p) + b1 * 50;
    return residue % p;
}


uint64_t multS(uint64_t a, uint64_t b, uint64_t p)
{
    uint64_t a1 = a;
    a1 = a1 >> 32;
    uint64_t b1 = 0xffffffff & a;
    uint64_t a2 = b;
    a2 = a2 >> 32;
    uint64_t b2 = 0xffffffff & b;
    uint64_t A = mult64_mod_pS(a1 * a2, p);
    uint64_t B = mult32_mod_pS(a1 * b2, p);
    uint64_t C = mult32_mod_pS(a2 * b1, p);
    uint64_t D = (b1 * b2) % p;

    return ((((A + B) % p + C) % p + D) % p);
}

//bool test_mult(uint64_t a, uint64_t b, uint64_t c)
//{
//    if (multS(a, b, p) == c)
//    {
//        return true;
//    }
//    else {
//        return false;
//    }
//}