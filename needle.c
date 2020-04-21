#include <math.h>

#include "needle.h"
#include "random-interface.h"

int drop_needle(needle *ndl, double t)
{
	if (range_randomd(0.0, t, &ndl->x) != 0 ||
	    range_randomd(0.0, 360.0, &ndl->angle) != 0)
		return -1;
	return 0;
}

int needle_crosses(needle *ndl, double l, double t)
{
	double pi = acos(-1);
	double a = l * cos(ndl->angle * pi / 180);
	if (ndl->x + fabs(a) > t) return 1;
	else return 0;
}
