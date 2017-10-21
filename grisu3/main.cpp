#include <iostream>
#include "grisu3.h"

using namespace std;

int main()
{
    double value = 12;
    int count = 3;
    int dec;
    int sign = 0;
    wchar_t digits[51];
    bool r = Grisu3::Run(value, count, &dec, &sign, digits);

    return 0;
}