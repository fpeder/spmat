#include "spmat.h"
#include "point.h"

#include <stdio.h>

#define RORB(x, n)         ((x) >> n) | ((x) << (8-n))

static const uint8_t map[] = {4, 5, 6, 7, 0, 1, 2, 3};
static const int8_t di[]   = {0,  +1, +1, +1,  0, -1, -1, -1};
static const int8_t dj[]   = {-1, -1,  0, +1, +1, +1,  0, -1};



static inline
uint8_t find_next(uint8_t neigh, uint8_t prev)
{
     __asm__ __volatile__ (
          "mov    %0,%%cl\n"
          "rorb   %%cl,%1\n"
          "movzbl %1,%%eax\n"
          "bsr    %%eax,%%eax\n"
          "addb   %%al,%0\n"
          "andb   $7,%0\n"
          : "+r" (prev)
          : "r" (neigh)
          : "%cl", "%eax"
          );
     return prev;
}


void spmat_contour(spmat in, uint32_t *contour)
{
     uint8_t *data = (uint8_t *) in.ngh.data;
     uint8_t next, prev=0;
     uint32_t start=1, end=0;
     int i, j, k=0;

     spmat_first(in, &i, &j);
     start = POINT_PACK(i, j);
     while (start != end) {
          next = find_next(data[i*in.N + j], prev);
          i += di[next]; j += dj[next];
          end = POINT_PACK(i, j);
          contour[k++] = end;
          prev = map[next];
     }

}
