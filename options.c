#include "options.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void parseOptions(int argc, char **argv, struct options* opt)
{
    int c;
    char *i_input;
    char *o_input;
    bool *valid;
    long long *nbytes;
     while ((c = getopt(argc, argv, "i:o:")) != -1) {
         switch('c')
         {
             case 'i':  
             i_input = optarg; 
             break;
             case 'o':
             o_input = optarg;
             break;
             default: break;
        }
         
     }
     if (argv[optind] != NULL)
     {
      char *endptr;
      errno = 0;
      *nbytes = strtoll (argv[optind], &endptr, 10);
      if (errno)
        perror (argv[optind]);
      else
        *valid = !*endptr && 0 <= *nbytes;
    }

}