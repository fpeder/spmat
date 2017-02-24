#include <stdio.h>
#include <stdlib.h>

#include "all.h"


static const
char usage[] = "usage: ./contour <src>\n";


int main(int argc, char *argv[])
{
     uint32_t contour[100000];
     spmat src = spmat_read(argv[1], 3);

     if (argc != 2) {
          fprintf(stderr, usage);
          exit(-1);
     }

     //spmat_print(src);
     //spmat_print_neigh(src);

     printf("contour ");

     TIMER_START(1000);
     spmat_contour(src, contour);
     TIMER_STOP(1000);

     spmat_free(src);
     return 0;
}
