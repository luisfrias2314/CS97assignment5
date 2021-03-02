#ifndef MRAND48_R_H
#define MRAND48_R_H


#include <errno.h>
#include <immintrin.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void mrand48_rng_init(char *input);
unsigned long long mrand48_rng(void);
void mrand48_rng_fini(void);

#endif