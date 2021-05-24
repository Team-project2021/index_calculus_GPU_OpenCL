#include <iostream>
#include <stdio.h>
#include <cassert>
#include "Header.h"


int main()
{
    printf("%llu\n", mult(2345676543234, 23456743234));
    printf("%llu\n", 4518184817900433161);
    printf("%llu\n", mult(1ull << 32, 1ull << 32));
    printf("%llu", 50ull);

    return 0;


}

