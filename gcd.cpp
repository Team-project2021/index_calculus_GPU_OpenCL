#include <stdio.h>
#include <stdint.h>

//unsigned long long gcdExtended(unsigned long long a, unsigned long long b, unsigned long long* x, unsigned long long* y) {
//    if (a == 0) {
//        *x = 0;
//        *y = 1;
//        return b;
//    }
//    unsigned long long x1, y1;
//    unsigned long long gcd = gcdExtended(b % a, a, &x1, &y1);
//    *x = y1 - (b / a) * x1;
//    *y = x1;
//    return gcd;
//}
//
//bool test_gcdExtended(unsigned long long a, unsigned long long b, unsigned long long c)
//{
//    unsigned long long x, y;
//    if (gcdExtended(a, b, &x, &y) == c)
//        return true;
//    else
//        return false;
//}