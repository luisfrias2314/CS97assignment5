#include "mrand48-r.h"
#include <time.h>
struct drand48_data buffer = {0};

void mrand48_rng_init(char *input) {
    srand48_r(time(NULL), &buffer);
}

unsigned long long mrand48_rng(void)
{
    long int x;
    mrand48_r(&buffer, &x);
    unsigned long long num;
    num = (unsigned long long) x;
    return num;
}

void mrand48_rng_fini(void)
{

}