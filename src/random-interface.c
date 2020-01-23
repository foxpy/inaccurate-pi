#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
#define _CRT_RAND_S
#endif

#ifdef __linux__
#include <sys/random.h>
#endif

#include "random-interface.h"

#define DOUBLE_ONE_EXPONENT 0x3ff0000000000000
#define DOUBLE_FRACTION_MASK 0xfffffffffffff
#define RAND_BUF_SIZE 4096

union transparent_double {
	double d;
	uint64_t u;
};
typedef union transparent_double td;

static int platform_random(void *dst, size_t len)
{
	static uint8_t rand_buf[RAND_BUF_SIZE];
	static size_t pos = RAND_BUF_SIZE;
	size_t n;
	uint8_t *data;
#ifdef _WIN32
	size_t i;
#endif

	data = dst;
rand_start:
	if (pos == RAND_BUF_SIZE) {
#ifdef __linux__
		if (getrandom(&rand_buf[0],
		              sizeof(uint8_t)*RAND_BUF_SIZE, 0) == -1)
			return -1;
#elif defined _WIN32
/* works only on perfectly aligned cache size == sizeof(unsigned)*N
 * where N = 1, 2, 3, ...
 */
		for (i = 0; i < RAND_BUF_SIZE; i += sizeof(unsigned)) {
			if (rand_s(&rand_buf[i]) != 0) return -1;
		}
#endif
		pos = 0;
	}

	n = (len <= RAND_BUF_SIZE - pos) ? len : RAND_BUF_SIZE - pos;
	memcpy(data, &rand_buf[pos], n);
	data += n;
	pos += n;
	len -= n;
	if (len != 0) goto rand_start;
	return 0;
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
