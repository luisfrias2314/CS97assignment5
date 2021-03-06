/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <cpuid.h>
#include <errno.h>
#include <immintrin.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "rand64-sw.h"
#include "rand64-hw.h"
#include "output.h"
#include "options.h"
#include <string.h>
#include <unistd.h>
#include "mrand48-r.h"

/* Hardware implementation.  */

/* Description of the current CPU.  */
struct cpuid { unsigned eax, ebx, ecx, edx; };

/* Return information about the CPU.  See <http://wiki.osdev.org/CPUID>.  */
static struct cpuid
cpuid (unsigned int leaf, unsigned int subleaf)
{
  struct cpuid result;
  asm ("cpuid"
       : "=a" (result.eax), "=b" (result.ebx),
	 "=c" (result.ecx), "=d" (result.edx)
       : "a" (leaf), "c" (subleaf));
  return result;
}

/* Return true if the CPU supports the RDRAND instruction.  */
static _Bool
rdrand_supported (void)
{
  struct cpuid extended = cpuid (1, 0);
  return (extended.ecx & bit_RDRND) != 0;
}

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{

  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  struct options opt = {};
  parseOptions(argc,argv,&opt);
  valid = opt.valid;
  nbytes = opt.nbytes;
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void (*initialize) (char *input);
  unsigned long long (*rand64) (void);
  void (*finalize) (void);
    if(opt.first_input != NULL)
    {
      if(strcmp(opt.first_input,"rdrand") == 0)
      {
        if (rdrand_supported ())
        {
          initialize = hardware_rand64_init;
          rand64 = hardware_rand64;
          finalize = hardware_rand64_fini;
         }
         else
         {
           fprintf (stderr, "rdrand not supported");
         }
      }
      else if(strcmp(opt.first_input,"mrand48_r") == 0)
      {
      initialize = mrand48_rng_init;
      rand64 = mrand48_rng;
      finalize = mrand48_rng_fini;
      }
      else
      {
        initialize = software_rand64_init;
        rand64 = software_rand64;
        finalize = software_rand64_fini;
      }

    }
    else
    {
       if (rdrand_supported ())
       {
         
         initialize = hardware_rand64_init;
         rand64 = hardware_rand64;
         finalize = hardware_rand64_fini;
       }
       else
       {
         fprintf (stderr, "rdrand not supported");
       }
    }

  initialize (opt.first_input);
  int wordsize = sizeof rand64 ();
  int output_errno = 0;

if(opt.second_input != NULL)
{
  if(strcmp(opt.second_input,"stdio" ) != 0)
  {
    long long int totalwritten = 0;
    long long int requiredToWrite = nbytes;
    int bufferSize = atoi(opt.second_input);
    char *numbers = malloc(bufferSize * sizeof(char));
    if(numbers==NULL)                     
    {
        exit(1);
    }
    while(totalwritten < requiredToWrite)
    {
      unsigned long long x = rand64 ();
      int currentArrayIndex = 0;
      if(totalwritten + bufferSize > requiredToWrite)
      {
        bufferSize = requiredToWrite - totalwritten;
      }
      while (x > 0 && currentArrayIndex < bufferSize)
      {
        numbers[currentArrayIndex] = x;
        currentArrayIndex++;
        x = (x >>= 1);
      }
      
      if(bufferSize == currentArrayIndex)
      {
        int byteswritten = write(1,numbers,bufferSize);
        totalwritten += byteswritten;
      }
    } 
  }
  else
  {
    do
  {
      unsigned long long x = rand64 ();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes (x, outbytes))
	{
	  output_errno = errno;
	  break;
	}
      nbytes -= outbytes;
  }
  while (0 < nbytes);
  
  }
}
else 
{
  do
  {
      unsigned long long x = rand64 ();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes (x, outbytes))
	{
	  output_errno = errno;
	  break;
	}
      nbytes -= outbytes;
  }
  while (0 < nbytes);
}

  
  
  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
    }

  finalize ();
  return !!output_errno;
}
