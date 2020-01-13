#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include "needle.h"

static void help(char *program_name);

int main(int argc, char *argv[])
{
	double l, t;
	long long n;
	needle ndl;

	if (argc != 4) {
		help(argv[0]);
		return EXIT_FAILURE;
	}

	l = atof(argv[1]);
	t = atof(argv[2]);
	n = atoll(argv[3]);
	if (l <= DBL_EPSILON) {
		fprintf(stderr, "Fatal: l too small\n");
		return EXIT_FAILURE;
	}
	if (t <= DBL_EPSILON) {
		fprintf(stderr, "Fatal: t too small\n");
		return EXIT_FAILURE;
	}
	if (n <= 0) {
		fprintf(stderr, "Fatal: expected positive n\n");
		return EXIT_FAILURE;
	}
	if (l >= t) {
		fprintf(stderr, "Fatal: expected l < t\n");
		return EXIT_FAILURE;
	}

	while (n-- > 0) {
		if (drop_needle(&ndl, t) != 0) return EXIT_FAILURE;
		printf("Dropped needle: %f:%f, %f deg\n",
		       ndl.x, ndl.y, ndl.angle);
	}

	return EXIT_SUCCESS;
}

static void help(char *program_name)
{
	fprintf(stderr, "Usage: %s l t n\n", program_name);
	fprintf(stderr, "Where:\n");
	fprintf(stderr, "\tl - length of needle (floating point)\n");
	fprintf(stderr, "\tt - distance between lines (floating point)\n");
	fprintf(stderr, "\tn - number of needles (integer)\n");
}
