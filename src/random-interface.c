#ifdef _WIN32
#define _CRT_RAND_S
#include <string.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifdef __linux__
#include <sys/random.h>
#endif

#include "random-interface.h"

#define DOUBLE_ONE_EXPONENT 0x3ff0000000000000
#define DOUBLE_FRACTION_MASK 0xfffffffffffff
#define abs(x) ((x>=0)?x:-x)

union transparent_double {
	double d;
	uint64_t u;
};
typedef union transparent_double td;

static int platform_random(void *dst, size_t len)
{
#ifdef _WIN32
	unsigned rc;
	if (len < sizeof(unsigned)) {
		if (rand_s(&rc) != 0) return -1;
		memcpy(dst, &rc, len);
		return 0;
	}

	for (size_t i = 0; i < len / sizeof(unsigned); i += sizeof(unsigned)) {
		if (rand_s(((unsigned*) dst) + i) != 0) return -1;
	}
	if (len % sizeof(unsigned)) {
		if (rand_s((uncigned*) dst + len - sizeof(unsigned)) != 0)
			return -1;
	}
#endif

#ifdef __linux__
	if (getrandom(dst, len, 0) == (ssize_t) len) return 0;
#endif

	return 1;
}

int range_randomd(double low, double high, double *dst)
{
	td val;

	if (low >= high) return -1;
	if (platform_random(&val.d, sizeof(val.d)) != 0) return -1;
	val.u &= DOUBLE_FRACTION_MASK;
	val.u |= DOUBLE_ONE_EXPONENT;
	val.d -= 1;
	*dst = low + val.d * (high - low);
	return 0;
}

int range_randoms(signed low, signed high, signed *dst)
{
	unsigned u;

	if (low >= high) return -1;
	if (platform_random(&u, sizeof(u)) != 0) return -1;
	u %= abs(low) + abs(high);
	*dst = (low>=0) ? u - low : u + low;
	return 0;
}
