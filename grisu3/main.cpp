#include <iostream>
#include "grisu3.h"

using namespace std;

int main()
{
    for (int count = 17; count <= 17; ++count)
    {
        uint64_t success = 0;
        uint64_t v = 0;
        int dec;
        int sign = 0;
        wchar_t digits[51];
        double value = 0;
        for (; v < UINT64_MAX; ++v)
        {
            value = *((double*)&v);
            if (((FPDOUBLE*)&value)->exp == 0x7FF || value == 0.0)
            {
                ++success;
                continue;
            }

            if (value < 0)
            {
                value = -value;
            }
            
            if (Grisu3::Run(value, count, &dec, &sign, digits))
            {
                ++success;
            }

            if (v % 10000000 == 0)
            {
                double rate = (double)success / (double)v * 100;
                printf("The complete Grisu3 hit rate for precision count %d in %llu numbers is %.2f\n", count, v, rate);
            }
        }

        value = *((double*)&v);
        if (((FPDOUBLE*)&value)->exp == 0x7FF || value == 0.0)
        {
            ++success;
            continue;
        }

        if (Grisu3::Run(value, count, &dec, &sign, digits))
        {
            ++success;
        }

        double rate = (double)success / (double)UINT64_MAX * 100;
        printf("The complete Grisu3 hit rate for precision count %d is %.2f\n", count, rate);
    }

    return 0;
}