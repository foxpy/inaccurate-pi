#ifdef _WIN32
#define _CRT_RAND_S
#endif

#ifdef __linux__
#include <sys/random.h>
#define RAND_BUF_SIZE 4096
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "random-interface.h"

#define DOUBLE_ONE_EXPONENT 0x3ff0000000000000
#define DOUBLE_FRACTION_MASK 0xfffffffffffff

union transparent_double {
	double d;
	uint64_t u;
};
typedef union transparent_double td;

#ifdef _WIN32
static int platform_random(void *dst, size_t len)
{
	unsigned rc;
	size_t i;
	if (len < sizeof(unsigned)) {
		if (rand_s(&rc) != 0) return -1;
		memcpy(dst, &rc, len);
		return 0;
	}

	for (i = 0; i < len / sizeof(unsigned); i += sizeof(unsigned)) {
		if (rand_s(((unsigned*) dst) + i) != 0) return -1;
	}
	if (len % sizeof(unsigned)) {
		if (rand_s((unsigned*) dst + len - sizeof(unsigned)) != 0)
			return -1;
	}
	return 0;
}
#endif

#ifdef __linux__
static int platform_random(void *dst, size_t len)
{
	static uint8_t rand_buf[RAND_BUF_SIZE];
	static size_t pos = RAND_BUF_SIZE;
	size_t n;
	uint8_t *data;

	data = dst;
rand_start:
	if (pos == RAND_BUF_SIZE) {
		if (getrandom(&rand_buf[0],
		              sizeof(uint8_t)*RAND_BUF_SIZE, 0) == -1)
			return -1;
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
#endif

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
