#include <stdio.h>
#include "Header.h"
#include "Header1.h"
#include "Header2.h"
#include "gcd.cpp"
#include <CL/cl.h>
#include <vector>
#include "matrix.h"
#include <cmath>
#include <tuple>
#include <time.h>
#include <math.h>



bool is_integer(float k)
{
    if (k == (uint64_t)k) return true;
    return false;
}

tuple<std::vector<uint64_t>, std::vector<uint64_t>> factor(uint64_t N){
    std::vector<uint64_t> factors;
    std::vector<uint64_t> powers;
    std::tuple<std::vector<uint64_t>, std::vector<uint64_t>> score;
    int  i = 2;
    bool pom;
    while(N > 1){
        if(N%i == 0) {
            N = N / i;
            //std::cout << N << endl;
            pom = true;
            for (int j = 0; j < factors.size(); j++) {
                if (factors[j] == i) {
                    powers[j] = powers[j] + 1;
                    pom = false;

                    }
           
                }
            if (pom == true) {
                factors.push_back(i);
                powers.push_back(1);
                
            }
            i = i - 1;
            
        }
        i = i + 1;
        
    }
    score = std::make_tuple(factors, powers);
    
   
    return score;
}

void print_vector(vector<uint64_t>& vec) {
    for (int i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
}

void relacje(vector<uint64_t> vec, uint64_t gen, uint64_t q) {
    int size = vec.size();
    uint64_t exp, gen_temp;
    tuple<std::vector<uint64_t>, std::vector<uint64_t>> wynik;
    std::vector<uint64_t> powers;
    std::vector<uint64_t> factors;
    srand((unsigned)time(NULL));
    for (int i = 0; i < size; i++) {
        exp = rand() % q; // spr
        gen_temp = ((uint64_t)pow(gen, exp)) % q;
        tie(factors, powers) = factor(gen_temp);
        std::cout << "-------="<< i << endl;
        std::cout<<gen <<" "<< exp<<" " << gen_temp << endl;
        print_vector(factors);
        std::cout << endl;
        print_vector(powers);



       





    }



}

int main()
{
    /*Matrix m(5);
    m.add_row({ 1, 2, 3, 4, 5 });
    m.add_row({ 6, 7, 8, 9, 10});
    m.add_row({ 4, 44,6, 1, 88});
    m(2, 1) = 15;
    m.print_matrix();*/
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

    /*m.swap_rows(0, 1);

    m.print_matrix();*/
    std::vector<uint64_t> powers;
    std::vector<uint64_t> factors;

    tuple<std::vector<uint64_t>, std::vector<uint64_t>> wynik;
    

    std::vector<uint64_t> vec({ 2,3,5,7 });
    relacje(vec, 13,  47);

    
    return 0;
}

