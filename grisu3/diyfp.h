#ifndef _DIYFP_H
#define _DIYFP_H

#include <stdint.h>
#include <assert.h>

struct FPDOUBLE
{
#if BIGENDIAN
    unsigned int sign : 1;
    unsigned int exp : 11;
    unsigned int mantHi : 20;
    unsigned int mantLo;
#else
    unsigned int mantLo;
    unsigned int mantHi : 20;
    unsigned int exp : 11;
    unsigned int sign : 1;
#endif
};

class DiyFp
{
public:
    DiyFp(uint64_t f, int e)
        : m_f(f), m_e(e)
    {
    }

    DiyFp(const DiyFp& rhs)
        : m_f(rhs.m_f), m_e(rhs.m_e)
    {
    }

    DiyFp& operator=(const DiyFp& rhs)
    {
        m_f = rhs.m_f;
        m_e = rhs.m_e;

        return *this;
    }

    uint64_t f() const
    {
        return m_f;
    }

    int e() const
    {
        return m_e;
    }

    void Minus(const DiyFp& rhs);
    static DiyFp Minus(const DiyFp& left, const DiyFp& right);

    void Multiply(const DiyFp& rhs);
    static DiyFp Multiply(const DiyFp& left, const DiyFp& right);

    static DiyFp GenerateNormalizedDiyFp(double value);
public:
    static const int SIGNIFICAND_LENGTH = 64;
private:
    uint64_t m_f;
    int m_e;
};

#endif