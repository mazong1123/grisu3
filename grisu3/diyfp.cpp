#include "diyfp.h"

void DiyFp::Minus(const DiyFp& rhs)
{
    assert(m_e == rhs.e());
    assert(m_f >= rhs.f());

    m_f -= rhs.f();
}

DiyFp DiyFp::Minus(const DiyFp& left, const DiyFp& right)
{
    DiyFp r = left;
    r.Minus(right);

    return r;
}

void DiyFp::Multiply(const DiyFp& rhs)
{
    uint64_t m32 = 0xFFFFFFFF;

    uint64_t a = m_f >> 32;
    uint64_t b = m_f & m32;
    uint64_t c = rhs.f() >> 32;
    uint64_t d = rhs.f() & m32;

    uint64_t ac = a * c;
    uint64_t bc = b * c;
    uint64_t ad = a * d;
    uint64_t bd = b * d;

    uint64_t tmp = (bd >> 32) + (ad & m32) + (bc & m32);
    tmp += 1U << 31;

    m_f = ac + (ad >> 32) + (bc >> 32) + (tmp >> 32);
    m_e = m_e + rhs.e() + SIGNIFICAND_LENGTH;
}

DiyFp DiyFp::Multiply(const DiyFp& left, const DiyFp& right)
{
    DiyFp r = left;
    r.Multiply(right);

    return r;
}

DiyFp DiyFp::GenerateNormalizedDiyFp(double value)
{
    assert(value > 0.0);

    uint64_t f = 0;
    int e = 0;
    uint64_t normalizeBit = (uint64_t)1 << 52;
    if (((FPDOUBLE*)&value)->exp != 0)
    {
        // For normalized value, according to https://en.wikipedia.org/wiki/Double-precision_floating-point_format
        // value = 1.fraction * 2^(exp - 1023) 
        //       = (1 + mantissa / 2^52) * 2^(exp - 1023) 
        //       = (2^52 + mantissa) * 2^(exp - 1023 - 52)
        //
        // So f = (2^52 + mantissa), e = exp - 1075; 
        f = ((uint64_t)(((FPDOUBLE*)&value)->mantHi) << 32) | ((FPDOUBLE*)&value)->mantLo + normalizeBit;
        e = ((FPDOUBLE*)&value)->exp - 1075;
    }
    else
    {
        // For denormalized value, according to https://en.wikipedia.org/wiki/Double-precision_floating-point_format
        // value = 0.fraction * 2^(1 - 1023)
        //       = (mantissa / 2^52) * 2^(-1022)
        //       = mantissa * 2^(-1022 - 52)
        //       = mantissa * 2^(-1074)
        // So f = mantissa, e = -1074
        f = ((uint64_t)(((FPDOUBLE*)&value)->mantHi) << 32) | ((FPDOUBLE*)&value)->mantLo;
        e = -1074;
    }

    while ((f & normalizeBit) == 0)
    {
        f <<= 1;
        --e;
    }

    int lengthDiff = DiyFp::SIGNIFICAND_LENGTH - 53;
    f <<= lengthDiff;
    e -= lengthDiff;

    return DiyFp(f, e);
}