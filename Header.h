#include <stdint.h>

const unsigned long long p = 9223372036854775783;

uint64_t mult32_mod_p(uint64_t a)
{
    a = a % p;
    uint64_t a1 = a >> 32;
    uint64_t b1 = 0xffffffff & a;
    uint64_t residue = ((b1 << 32) % p) + a1 * 50;
    return residue % p;
}

uint64_t mult64_mod_p(uint64_t a)
{
    a = a % p;
    uint64_t a1 = a >> 32;
    uint64_t b1 = 0xffffffff & a;
    uint64_t residue = mult32_mod_p(a1 * 50) + b1 * 50;
    return residue % p;
}


unsigned long long mult(unsigned long long a, unsigned long long b)
{
    unsigned long long a1 = a;
    a1 = a1 >> 32;
    unsigned long long b1 = 0xffffffff & a;
    unsigned long long a2 = b;
    a2 = a2 >> 32;
    unsigned long long b2 = 0xffffffff & b;
    uint64_t A = mult64_mod_p(a1 * a2);
    uint64_t B = mult32_mod_p(a1 * b2);
    uint64_t C = mult32_mod_p(a2 * b1);
    uint64_t D = (b1 * b2) % p;

    return ((((A + B) % p + C) % p + D) % p);
}

bool test_mult(unsigned long long a, unsigned long long b, unsigned long long c)
{
    if (mult(a, b) == c)
    {
        return true;
    }
    else {
        return false;
    }
}