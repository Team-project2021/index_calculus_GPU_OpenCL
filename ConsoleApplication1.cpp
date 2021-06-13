#include <stdio.h>
#include "Header.h"
#include "Header1.h"
#include "Header2.h"
#include "gcd.cpp"
#include <CL/cl.h>
#include <vector>
#include "matrix.h"

int main()
{   
    Matrix m(5);
    m.add_row({ 1, 2, 3, 4, 5 });
    m.add_row({ 6, 7, 8, 9, 10});
    m.add_row({ 4, 44,6, 1, 88});
    m(2, 1) = 15;
    m.print_matrix();
    /*int res = m.find_inv(1, 30);
    std::cout << res << "\n";
    res = m.find_inv(1, 210);
    std::cout << res << "\n";
    res = m.find_inv(2, 6);
    std::cout << res << "\n";
    res = m.find_inv(2, 9);
    std::cout << res << "\n";*/


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
    /*std::vector<cl_ulong> V({ 1,2,6,6,6,6,6,6,6 });
    vector_mult(V, 2);*/

    m.swap_rows(0, 1);

    m.print_matrix();

    return 0;

}

