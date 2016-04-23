#include "stdio.h"
#include "stdlib.h"
#include <sys/time.h>
int main () {

   int i;
   int t1;
   int t2;
   double newstart, newend;
   double mytime;
   struct timeval start, end;

   for ( i=0; i<10; i++ ) {
      gettimeofday(&start, NULL);
      sleep(1);
      gettimeofday(&end, NULL);
      newstart = start.tv_sec + (double)start.tv_usec / 1000000.0;
      newend = end.tv_sec + (double)end.tv_usec / 1000000.0;
      mytime = newend - newstart;
      printf("%f\n", mytime);
   }

}

