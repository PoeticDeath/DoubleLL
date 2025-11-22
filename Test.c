#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

static long long lltodouble(long long rll)
{
	unsigned long long ull = llabs(rll);

	if (!ull)
	{
		return 0;
	}

	unsigned long long mask = (unsigned long long)1 << 52;
	unsigned long long i = 0;
	for (; i < 52; i++)
	{
		if (ull & mask)
		{
			break;
		}
		else
		{
			mask >>= 1;
		}
	}

	unsigned long long oex = 52 - i + 1023;
	unsigned long long ex = mask;
	unsigned long long frac = ull;
	frac &= ~ex;
	frac <<= i;

	unsigned long long ll = rll < 0 ? 0x8000000000000000 : 0;
	ll |= (oex << 52) & 0x7ff0000000000000;
	ll |= frac & 0x000fffffffffffff;

	return ll;
}

static long long doubletoll(long long rll)
{
	bool sign = (rll & 0x8000000000000000) >> 63;
	unsigned long long oex = (rll & 0x7ff0000000000000) >> 52;
	unsigned long long frac = rll & 0x000fffffffffffff;

	if (!oex && !frac)
	{
		return 0;
	}

	oex -= 1023;
	unsigned long long ex = (unsigned long long)1 << oex;
	frac >>= 52 - oex;

	return (ex + frac) * (1 - 2 * sign);
}

int main()
{
    for (long long o = 0; o < ((long long)1 << 62) / ((long long)1 << 32); o++)
    {
        #pragma omp target parallel for
        for (long long i = o * ((long long)1 << 32); i < (o + 1) * ((long long)1 << 32); i++)
        {
            double p = i;
            long long d = 0;
            memmove(&d, &p, 8);
            long long o = lltodouble(i);
            long long t = doubletoll(o);
            if ((long long) p != t || d != o)
            {
                printf("Error: %lld, %lld, %lld, %lld\n", i, t, d, o);
            }
            if (i % 100000000 == 0)
            {
                printf("Done: %lld\n", i);
            }
        }
    }
    printf("Done\n");

    return 0;
}
