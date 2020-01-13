#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/random.h>

#include "random-interface.h"

#define abs(x) ((x>=0)?x:-x)

double range_randomd(double low, double high)
{
	return low;
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
