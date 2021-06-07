#include <stdio.h>
#include "Header.h"
#include "Header1.h"
#include "Header2.h"
#include "gcd.cpp"
#include <CL/cl.h>
#include <vector>

int main()
{

    //std::string S = kernel_load("VectorMult.cl");
    //std::cout << S;
 /*   unsigned int truths = 0;
    for (int i = 0; i < 100; i++)
    {
        if (test_mult(mult_A[i], mult_B[i], mult_C[i]) == true)
            truths++;
    }
    printf("%d\n", truths);

    unsigned long long x, y;
    unsigned int truths_gcd = 0;
    for (int i = 0; i < 100; i++)
    {
        if (test_gcdExtended(gcd_a[i], gcd_b[i],gcd_c[i]) == true)
            truths_gcd++;
    }
    printf("%d\n", truths_gcd);*/
    std::vector<cl_ulong> V({ 1,2,6,6,6,6,6,6,6 });
    vector_mult(V, 2);

    return 0;

}

