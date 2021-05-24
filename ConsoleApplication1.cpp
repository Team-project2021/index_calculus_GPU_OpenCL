#include <stdio.h>
#include "Header.h"
#include "Header1.h"

int main()
{

    unsigned int truths = 0;
    for (int i = 0; i < 100; i++)
    {
        if (test_mult(A[i], B[i], C[i]) == true)
            truths++;
    }
    printf("%d", truths);
    return 0;

}

