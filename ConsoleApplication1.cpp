#include <stdio.h>
#include "Header.h"
#include "Header1.h"
#include "Header2.h"
#include "gcd.cpp"
#include <CL/cl.h>
#include <vector>
#include "matrix.h"

using namespace std;

int main()
{
    const uint64_t p = 9223372036854775783;
    Matrix m(3);

    //m.add_row({ 6,3 });
    //m.add_row({ 3, 8 });
    m.add_row({ 5,0,2 });
    m.add_row({ 7,3,8 });
    m.add_row({ 9,14,1 });
    m.add_row({ 7,15,15 });

    uint64_t a = 3;
    uint64_t b = 7;
    int x, y;

    uint64_t q = 2147483647;
    m.print_matrix();
    cout << endl;

    vector<uint64_t> V({ 1,2,6 });

    x = m.gauss(V, q);


    //m.print_matrix();
    cout << endl << x << endl;
    return 0;

}

