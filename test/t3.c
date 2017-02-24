#include <stdio.h>
#include <stdlib.h>

#include "all.h"

static const
char usage[] = "usage: ./gh <src> <dst>\n";


int main(int argc, char *argv[])
{
     spmat src = spmat_read(argv[1], 3);
     spmat dst = spmat_alloc_copy(src);
     int ntimes = 1;

     if (argc != 3) {
          fprintf(stderr, usage);
          exit(-1);
     }

     TIMER_START(ntimes);
     spmat_guohall(src, dst, gh_iter_nolookup);
     TIMER_STOP(ntimes);

     spmat_copy(src, dst);

     TIMER_START(ntimes);
     spmat_guohall(src, dst, gh_iter_lookup);
     TIMER_STOP(ntimes);

     spmat_copy(src, dst);

     /* TIMER_START(ntimes); */
     /* spmat_guohall(src, dst, gh_iter_plookup); */
     /* TIMER_STOP(ntimes); */

     spmat_write(argv[2], dst);
     spmat_free(src);
     spmat_free(dst);
     return 0;
}
