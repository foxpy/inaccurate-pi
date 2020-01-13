#ifndef __INACCURATE_PI_NEEDLE_H
#define __INACCURATE_PI_NEEDLE_H

struct needle {
	double x;
	double angle; /* deg */
};
typedef struct needle needle;

int drop_needle(needle *ndl, double l);

#endif
