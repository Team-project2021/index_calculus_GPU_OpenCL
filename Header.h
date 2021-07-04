#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <climits>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "matrix.h"

uint64_t mult32_mod_p(uint64_t a)
{
    const uint64_t p = 9223372036854775783;
    a = a % p;
    uint64_t a1 = a >> 32;
    uint64_t b1 = 0xffffffff & a;
    uint64_t residue = ((b1 << 32) % p) + a1 * 50;
    return residue % p;
}

uint64_t mult64_mod_p(uint64_t a)
{
    const uint64_t p = 9223372036854775783;
    a = a % p;
    uint64_t a1 = a >> 32;
    uint64_t b1 = 0xffffffff & a;
    uint64_t residue = mult32_mod_p(a1 * 50) + b1 * 50;
    return residue % p;
}


uint64_t mult(uint64_t a, uint64_t b)
{
    const uint64_t p = 9223372036854775783;
    uint64_t a1 = a;
    a1 = a1 >> 32;
    uint64_t b1 = 0xffffffff & a;
    uint64_t a2 = b;
    a2 = a2 >> 32;
    uint64_t b2 = 0xffffffff & b;
    uint64_t A = mult64_mod_p(a1 * a2);
    uint64_t B = mult32_mod_p(a1 * b2);
    uint64_t C = mult32_mod_p(a2 * b1);
    uint64_t D = (b1 * b2) % p;

    return ((((A + B) % p + C) % p + D) % p);
}

bool test_mult(uint64_t a, uint64_t b, uint64_t c)
{
    if (mult(a, b) == c)
    {
        return true;
    }
    else {
        return false;
    }
}