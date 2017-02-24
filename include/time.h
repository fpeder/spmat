#ifndef SPMAT_TIME_H
#define SPMAT_TIME_H

#include <sys/time.h>

struct timeval ts, te;

#define TIME_START()        time_start();
#define TIME_STOP()         time_stop();
#define TIME_STOP_PRINT(n)  print_elapsed_time(n)

#define TIMER_START(n) \
     TIME_START(); \
     for (int i=0; i < n; i++) { 

#define TIMER_STOP(n) \
     } TIME_STOP_PRINT(n)

void time_start();
void time_stop();
long elapsed_time();
void print_elapsed_time(int n);

#endif /* SPMAT_TIME_H */
