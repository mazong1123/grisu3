#include "grisu3.h"
#include <math.h>

// 1/lg(10)
const double Grisu3::D_1_LOG2_10 = 0.30102999566398114;

void Grisu3::Run(double value, int count, int* dec, int* sign, wchar_t* digits)
{
    DiyFp w = DiyFp::GenerateNormalizedDiyFp(value);
    int mk = KComp(w.e() + DiyFp::SIGNIFICAND_LENGTH);
}

int Grisu3::KComp(int e)
{
    return static_cast<int>(ceil((ALPHA - e + DiyFp::SIGNIFICAND_LENGTH - 1) * D_1_LOG2_10));
}

DiyFp Grisu3::CachedPower(int k)
{
    return DiyFp(0, 0);
}