#ifndef __INACCURATE_PI_RANDOM_INTERFACE_H
#define __INACCURATE_PI_RANDOM_INTERFACE_H

enum random_function {
	RANDOM_FUNCTION_PLATFORM,
	RANDOM_FUNCTION_XORSHIFT
};

int set_random_function(enum random_function func);
int range_randomd(double low, double high, double *dst);

#endif
