#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/random.h>

#include "random-interface.h"

#define DOUBLE_ONE_EXPONENT 0x3ff0000000000000
#define DOUBLE_FRACTION_MASK 0xfffffffffffff
#define abs(x) ((x>=0)?x:-x)

union transparent_double {
	double d;
	uint64_t u;
};
typedef union transparent_double td;

int range_randomd(double low, double high, double *dst)
{
	td val;

	if (low >= high) return -1;
	if (getrandom(&val.d, sizeof(val.d), 0) != sizeof(val.d)) return -1;
	val.u &= DOUBLE_FRACTION_MASK;
	val.u |= DOUBLE_ONE_EXPONENT;
	val.d -= 1;
	*dst = low + val.d * (high - low); // fuck it I need [low, high) but not [0.0F, 1.0F) >:(
	return 0;
}

int range_randoms(signed low, signed high, signed *dst)
{
	unsigned u;

	if (low >= high) return -1;
	if (getrandom(&u, sizeof(u), 0) != sizeof(u)) return -1;
	u %= abs(low) + abs(high);
	*dst = (low>=0) ? u - low : u + low;
	return 0;
}
