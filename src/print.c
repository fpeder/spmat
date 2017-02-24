#include "print.h"
#include "point.h"

#include <stdio.h>

#define BIT(x, n)   (int)(((x) >> (n)) & 1)


void print64(uint64_t n)
{
     for (int i=0; i < 64; i++) {
          printf("%d", BIT(n, i));
          if ((i+1) % 8 == 0) SP;
     }
}


void print8(uint8_t n)
{
     for (int i=7; i >= 0; i--)
          printf("%d", BIT(n, i));
}


void spmat_print(spmat mat)
{
     for (int i=0; i < mat.M; i++) {
          for (int j=0; j < mat.N; j++) {
               int v = SPMAT_PIX_GET(mat, i, j);
               printf("%d", v);
          } NL;
     } NL;
}


void spmat_print_neigh(spmat mat)
{
     if (!mat.k) return;
     for (int i=0; i < mat.M; i++) {
          for (int j=0; j < mat.N; j++) {
               if (SPMAT_PIX_GET(mat, i, j)) {
                    printf("(%d,%d) -> ", i, j);
                    print8(SPMAT_NGH_GET(mat, i, j));
                    NL;
               }}}
}


void points_print(uint32_t *pt)
{
     int x, y;
     for (int i=0; pt[i] != 0; i++) {
          POINT_UNPACK(pt[i], x, y);
          POINT_PRINT(x, y);
     } NL;
}
