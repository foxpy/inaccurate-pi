#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <qc.h>
#include "needle.h"

static void help(void *help_data) {
    char* program_name = help_data;
    fprintf(stderr, "Usage: %s --length=DOUBLE --distance=DOUBLE --iterations=INTEGER\n", program_name);
    fputs("Where:\n", stderr);
    fputs("\t--length - length of needle\n", stderr);
    fputs("\t--distance - distance between lines\n", stderr);
    fputs("\t--iterations - number of needles dropped\n", stderr);
}

int main(int argc, char *argv[]) {
    double l = 0.0;
    double t = 0.0;
    size_t n = 0;
    qc_args* args = qc_args_new();
    qc_args_set_help(args, help, argv[0]);
    qc_args_double(args, "length", &l);
    qc_args_double(args, "distance", &t);
    qc_args_unsigned(args, "iterations", &n);
    char* err;
    if (qc_args_parse(args, argc, argv, &err) == -1) {
        fprintf(stderr, "Failed to parse arguments: %s\n", err);
        free(err);
        exit(EXIT_FAILURE);
    } else if (l == 0.0 || t == 0.0 || n == 0) {
        help(argv[0]);
        exit(EXIT_FAILURE);
    } else if (l <= DBL_EPSILON) {
        die("Fatal: l too small");
    } else if (t <= DBL_EPSILON) {
        die("Fatal: t too small");
    } else if (n <= 0) {
        die("Fatal: expected positive n");
    } else if (l >= t) {
        die("Fatal: expected l < t");
    }

    qc_rnd rnd;
    qc_rnd_init(&rnd);
    size_t crossed = 0;
    needle ndl;
    for (size_t i = 0; i < n; ++i) {
        drop_needle(&rnd, &ndl, t);
        if (needle_crosses(&ndl, l, t)) ++crossed;
    }

    if (crossed == 0) {
        printf("Any needle did not cross the line.\n");
    } else {
        double P = (double) crossed / n;
        double pi = (2 * l) / (t * P);
        printf("pi = %f\n", pi);
    }
    return EXIT_SUCCESS;
}
