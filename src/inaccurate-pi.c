#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "needle.h"
#include "random-interface.h"

static void help(char *program_name);

int main(int argc, char *argv[])
{
	double l, t, P, pi;
	long long n, i, crossed;
	needle ndl;

	if (argc == 5) {
		if (strcmp(argv[1], "--use-xorshift") == 0) {
			if (set_random_function(RANDOM_FUNCTION_XORSHIFT) == -1)
				return EXIT_FAILURE;
		} else {
			help(argv[0]);
			return EXIT_FAILURE;
		}
	} else if (argc != 4) {
		help(argv[0]);
		return EXIT_FAILURE;
	}

	l = atof(argv[argc - 3]);
	t = atof(argv[argc - 2]);
	n = atoll(argv[argc - 1]);
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

	crossed = 0;
	for (i = 0; i < n; ++i) {
		if (drop_needle(&ndl, t) != 0) return EXIT_FAILURE;
		if (needle_crosses(&ndl, l, t)) ++crossed;
	}

	if (crossed == 0) {
		printf("Any needle did not cross the line.\n");
	} else {
		P = (double) crossed / n;
		pi = (2 * l) / (t * P);
		printf("pi = %f\n", pi);
	}

	return EXIT_SUCCESS;
}

static void help(char *program_name)
{
	fprintf(stderr, "Usage: %s [--use-xorshift] l t n\n", program_name);
	fprintf(stderr, "Where:\n");
	fprintf(stderr, "\tl - length of needle (floating point)\n");
	fprintf(stderr, "\tt - distance between lines (floating point)\n");
	fprintf(stderr, "\tn - number of needles (integer)\n");
}
