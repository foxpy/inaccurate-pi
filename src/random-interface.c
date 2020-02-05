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
#define RAND_BUF_SIZE 4096
#endif

#include "random-interface.h"

#define DOUBLE_ONE_EXPONENT 0x3ff0000000000000
#define DOUBLE_FRACTION_MASK 0xfffffffffffff

union transparent_double {
	double d;
	uint64_t u;
};
typedef union transparent_double td;
typedef int (*Random_function) (uint64_t *dst);

static int _platform_random(uint64_t *dst);
static int _xorshift_random(uint64_t *dst);
static Random_function rnd = _platform_random;

uint64_t xorshift_state;

int set_random_function(enum random_function func) {
	switch (func) {
	case RANDOM_FUNCTION_PLATFORM:
		rnd = _platform_random;
		break;
	case RANDOM_FUNCTION_XORSHIFT:
		if (_platform_random(&xorshift_state) == -1)
			return -1;
		rnd = _xorshift_random;
		break;
	}
	return 0;
}

#ifdef __linux__
static int _platform_random(uint64_t *dst)
{
	static uint64_t rand_buf[RAND_BUF_SIZE/sizeof(uint64_t)];
	static uint64_t pos = RAND_BUF_SIZE/sizeof(uint64_t);

	if (pos == RAND_BUF_SIZE/sizeof(uint64_t)) {
		if (getrandom(&rand_buf[0], sizeof(rand_buf), 0) == -1)
			return -1;
		pos = 0;
	}

	*dst = rand_buf[pos++];
	return 0;
}
#elif defined _WIN32
static int _platform_random(uint64_t *dst)
{
	uint8_t *u;
	u = dst;
	if (rand_s((uint8_t*) u) != 0 || rand_s((uint8_t*) u+4)) return -1;
}
#endif

static int _xorshift_random(uint64_t *dst)
{
	xorshift_state ^= (xorshift_state << 13);
	xorshift_state ^= (xorshift_state >> 7);
	xorshift_state ^= (xorshift_state << 17);
	*dst = xorshift_state;
	return 0;
}

int range_randomd(double low, double high, double *dst)
{
	td val;

	if (low >= high) return -1;
	if (rnd(&val.u) != 0) return -1;
	val.u &= DOUBLE_FRACTION_MASK;
	val.u |= DOUBLE_ONE_EXPONENT;
	val.d -= 1;
	*dst = low + val.d * (high - low);
	return 0;
}
