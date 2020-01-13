#include "needle.h"
#include "random-interface.h"

int drop_needle(needle *ndl, double t)
{
	if (range_randomd(0, t, &ndl->x) != 0 ||
	    range_randomd(0, 360.0, &ndl->angle) != 0) return -1;
	return 0;
}
