#include "all.h"

#include <stdlib.h>
#include <stdio.h>


static const
char usage[] = "usage: ./erode <src> <dst> \n";


int main(int argc, char *argv[])
{
     spmat src = spmat_read(argv[1], 0);
     spmat dst = spmat_alloc_copy(src);

     if (argc != 3) {
          fprintf(stderr, usage);
          exit(-1);
     }

     printf("e3x3 ");
     TIMER_START(NTIMES);
     spmat_erode(src, dst, 3);
     TIMER_STOP(NTIMES);

     //spmat_print(dst);

     printf("e5x5 ");
     TIMER_START(NTIMES);
     spmat_erode(src, dst, 5);
     TIMER_STOP(NTIMES);

     //spmat_print(dst);
     
     printf("e7x7 ");
     TIMER_START(NTIMES);
     spmat_erode(src, dst, 7);
     TIMER_STOP(NTIMES);

     //spmat_print(dst);

     spmat_write(argv[2], dst);
     spmat_free(src);
     spmat_free(dst);
     return 0;
}
