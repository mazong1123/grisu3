#include <iostream>
#include "grisu3.h"

using namespace std;

int main()
{
    double value = 19;
    int count = 17;
    int dec;
    int sign = 0;
    wchar_t digits[51];
    bool r = Grisu3::Run(value, count, &dec, &sign, digits);

    return 0;
}