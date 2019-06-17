#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/random.h>

#include "random-interface.h"

double range_randomd(double low, double high)
{
	double random;
start:
	if (getrandom(&random, sizeof(random), 0) != sizeof(random)) {
		perror("getrandom");
		exit(EXIT_FAILURE);
	}
	uint64_t exponent = *((uint64_t*)&random) & EXPONENT_MASK_DOUBLE;

	// get rid of subnormals, infinities and NaNs
	if ((exponent == INFINITY_EXPONENT_DOUBLE) ||
	    (exponent == SUBNORMAL_EXPONENT_DOUBLE)) {
		goto start;
	}
	return random;
}
