#include <stdio.h>
#include "Header.h"
#include "Header1.h"
#include "Header2.h"
#include "gcd.cpp"

int main()
{

    unsigned int truths = 0;
    for (int i = 0; i < 100; i++)
    {
        if (test_mult(mult_A[i], mult_B[i], mult_C[i]) == true)
            truths++;
    }
    printf("%d\n", truths);

    unsigned long long x, y;
    //unsigned long long gcd = gcdExtended(gcd/*_a[2], gcd_b[2], &xd, &yd);
    //printf("%llu, %llu, %llu", gcd, xd, yd);*/
    unsigned int truths_gcd = 0;
    for (int i = 0; i < 100; i++)
    {
        if (test_gcdExtended(gcd_a[i], gcd_b[i],gcd_c[i]) == true)
            truths_gcd++;
    }
    printf("%d\n", truths_gcd);
    return 0;

}

