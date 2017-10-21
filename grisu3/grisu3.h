#ifndef _GRISU3_H
#define _GRISU3_H

#include "diyfp.h"

class Grisu3
{
public:
    static void Run(double value, int count, int* dec, int* sign, wchar_t* digits);
private:
    Grisu3();
    Grisu3(const Grisu3&);
    Grisu3& operator=(const Grisu3&);

    static int KComp(int e);
    static DiyFp CachedPower(int k);

    // 1/lg(10)
    static const double D_1_LOG2_10;

    static const int ALPHA = -59;
    static const int GAMA = -32;
};

#endif