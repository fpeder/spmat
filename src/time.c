#include "time.h"

#include <stdio.h>


void time_start()
{
     gettimeofday(&ts, NULL);
}


void time_stop()
{
     gettimeofday(&te, NULL);
}


long elapsed_time()
{
     long sec, microsec;
     sec = te.tv_sec - ts.tv_sec;
     microsec = sec*1e6 + te.tv_usec - ts.tv_usec;
     return microsec;
}


void print_elapsed_time(int n)
{
     time_stop();
     printf("%.3f us\n", ((double)elapsed_time()) / n);
}
