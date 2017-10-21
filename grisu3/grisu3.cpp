#include "grisu3.h"
#include <math.h>

// 1/lg(10)
const double Grisu3::D_1_LOG2_10 = 0.30102999566398114;

bool Grisu3::Run(double value, int count, int* dec, int* sign, wchar_t* digits)
{
    if (value < 0)
    {
        value = -value;
        *sign = 1;
    }
    else
    {
        *sign = 0;
    }

    DiyFp w = DiyFp::GenerateNormalizedDiyFp(value);
    int mk = KComp(w.e() + DiyFp::SIGNIFICAND_LENGTH);

    DiyFp cmk;
    int decimalExponent;
    CachedPower(mk, &cmk, &decimalExponent);

    DiyFp D = DiyFp::Multiply(w, cmk);

    int kappa;
    int length;
    bool isSuccess = DigitGen(D, count, digits, &length, &kappa);

    digits[count] = 0;
    *dec = -decimalExponent + kappa;

    return isSuccess;
}

bool Grisu3::RoundWeed(wchar_t* buffer,
    int len,
    uint64_t rest,
    uint64_t tenKappa,
    uint64_t ulp,
    int* kappa)
{
    assert(rest < tenKappa);

    if (ulp >= tenKappa)
    {
        return false;
    }

    if (tenKappa - ulp <= ulp)
    {
        return false;
    }

    if ((tenKappa - rest > rest) && (tenKappa - 2 * rest >= 2 * ulp))
    {
        return true;
    }

    // If 2 * (rest - unit) >= 10^kappa, then we can safely round up.
    if ((rest > ulp) && (tenKappa - (rest - ulp) <= (rest - ulp)))
    {
        // Increment the last digit recursively until we find a non '9' digit.
        buffer[len - 1]++;
        for (int i = len - 1; i > 0; --i)
        {
            if (buffer[i] != L'0' + 10)
            {
                break;
            }

            buffer[i] = L'0';
            buffer[i - 1]++;
        }

        // If the first digit is now '0'+ 10 we had a buffer with all '9's. With the
        // exception of the first digit all digits are now '0'. Simply switch the
        // first digit to '1' and adjust the kappa. Example: "99" becomes "10" and
        // the power (the kappa) is increased.
        if (buffer[0] == L'0' + 10)
        {
            buffer[0] = L'1';
            (*kappa) += 1;
        }

        return true;
    }

    return false;
}

bool Grisu3::DigitGen(DiyFp mp, int count, wchar_t* buffer, int* len, int* K)
{
    assert(mp.e() >= ALPHA && mp.e() <= GAMA);

    uint64_t ulp = 1;
    DiyFp one = DiyFp(static_cast<uint64_t>(1) << -mp.e(), mp.e());
    uint32_t p1 = static_cast<uint32_t>(mp.f() >> -one.e());
    uint64_t p2 = mp.f() & (one.f() - 1);

    int length = 0;
    int kappa = 10;
    int div = TEN9;

    while (kappa > 0)
    {
        int d = p1 / div;
        if (d != 0 || length != 0)
        {
            buffer[length++] = L'0' + d;
            --count;
        }

        p1 %= div;
        --kappa;

        if (count == 0)
        {
            break;
        }

        div /= 10;
    }

    if (count == 0)
    {
        uint64_t rest = (static_cast<uint64_t>(p1) << -one.e()) + p2;

        *len = length;
        *K = kappa;

        return RoundWeed(buffer,
            length,
            rest,
            static_cast<uint64_t>(div) << -one.e(),
            ulp,
            K);
    }

    while (count > 0 && p2 > ulp)
    {
        p2 *= 10;

        int d = static_cast<int>(p2 >> -one.e());
        if (d != 0 || length != 0)
        {
            buffer[length++] = L'0' + d;
            --count;
        }

        p2 &= one.f() - 1;
        --kappa;

        ulp *= 10;
    }

    if (count != 0)
    {
        return false;
    }

    *len = length;
    *K = kappa;

    return RoundWeed(buffer, length, p2, one.f(), ulp, K);
}

int Grisu3::KComp(int e)
{
    return static_cast<int>(ceil((ALPHA - e + DiyFp::SIGNIFICAND_LENGTH - 1) * D_1_LOG2_10));
}

void Grisu3::CachedPower(int k, DiyFp* cmk, int* decimalExponent)
{
    assert(cmk != NULL);

    int index = (POWER_OFFSET + k - 1) / POWER_DECIMAL_EXPONENT_DISTANCE + 1;
    Power cachedPower = m_cachedPowers[index];

    cmk->SetSignificand(cachedPower.significand);
    cmk->SetExponent(cachedPower.binaryExponent);
    *decimalExponent = cachedPower.decimalExponent;
}