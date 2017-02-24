#include "all.h"



int main(int argc, char *argv[])
{
     uint32_t rl[100000];
     spmat src = spmat_read(argv[1], 0);

     TIMER_START(1000);
     spmat_rl(src, rl);
     TIMER_STOP(1000);

     //points_print(rl);
     spmat_free(src);
     return 0;
}

