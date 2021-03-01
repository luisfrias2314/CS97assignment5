#ifndef OPTIONS_H
#define OPTIONS_H
#include <stdbool.h>

struct options{
    char *first_input;
    char *second_input;
    long long nbytes;
    bool valid;
};
void parseOptions(int argc, char **argv, struct options* opt);


#endif
