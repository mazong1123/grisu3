#include <iostream>
#include "grisu3.h"

using namespace std;

int main()
{
    double value = 1.23;
    int count = 17;
    int dec;
    int sign = 0;
    char digits[18];
    digits[17] = 0;
    bool r = Grisu3::Run(value, count, &dec, &sign, digits);

    return 0;
}