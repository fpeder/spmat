#include "spmat.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


//static void spmat_set_neigh(spmat mat, int i, int j);
static void spmat_update_neigh(spmat mat, int i, int j);


spmat spmat_init(int M, int N, int k)
{
     spmat mat;
     mat.M = M; mat.N = N, mat.k = k;
     mat.pix = pmat_init(M, N, 64);
     if (k) mat.ngh = pmat_init(M, N, 64/(k*k-1));
     return mat;
}


void spmat_free(spmat mat)
{
     pmat_free(mat.pix);
     if (mat.k) pmat_free(mat.ngh);
}


spmat spmat_alloc_copy(spmat in)
{
     spmat ou = spmat_init(in.M, in.N, in.k);
     memcpy(ou.pix.data, in.pix.data, sizeof(uint64_t) * in.pix.sz);
     if (in.k) memcpy(ou.ngh.data, in.ngh.data,
                      sizeof(uint64_t) * ou.ngh.sz);
     return ou;
}


void spmat_copy(spmat src, spmat dst)
{
     assert(src.M == dst.M && src.N == dst.N);
     memcpy(dst.pix.data, src.pix.data, sizeof(uint64_t) * src.pix.sz);
     if (src.k) memcpy(dst.ngh.data, src.ngh.data,
                       sizeof(uint64_t) * src.ngh.sz);
}


void spmat_set(spmat mat, int i, int j)
{
     SPMAT_PIX_SET(mat, i, j);
     if (mat.k) spmat_update_neigh(mat, i, j);
}


void spmat_del(spmat mat, int i, int j)
{
     SPMAT_PIX_DEL(mat, i, j);
     if (mat.k) spmat_update_neigh(mat, i, j);
}


void spmat_first(spmat mat, int *i, int *j)
{
     int k=0, o; uint64_t ppix;
     while (k < mat.pix.M * mat.pix.N) {
          if ((ppix = mat.pix.data[k])) {
               o = __builtin_ctzll(ppix);
               *i = k / mat.pix.N;
               *j = ((k % mat.pix.N) << 6) + o;
               break;
          }
          k++;
     }
}


int spmat_foreach(spmat mat, int *ii, int *jj)
{
     static int i=0; int o;
     static uint64_t mask;
     uint64_t ppix;
     while (i < mat.pix.M * mat.pix.N) {
          ppix = mat.pix.data[i] & mask;
          if (ppix) {
               o = __builtin_ctzll(ppix);
               mask = (1ULL <<  (63-o)) - 1;
               *ii = i / mat.pix.N;
               *jj = ((i % mat.pix.N) << 6) + o;
               return 1;
          } else {i++; mask = ~0;}
     }
     i=0; mask=~0;
     return 0;
}


static
void spmat_update_neigh(spmat mat, int i, int j)
{
     int k = j & 7;
     int off = (k - 1) << 3;
     switch (k) {
     case 0:
          SPMAT_PNGH_GET(mat, i-1, j-1) ^= 0x0800000000000000;
          SPMAT_PNGH_GET(mat, i  , j-1) ^= 0x1000000000000000;
          SPMAT_PNGH_GET(mat, i+1, j-1) ^= 0x2000000000000000;
          SPMAT_PNGH_GET(mat, i-1, j)   ^= 0x0204ULL;
          SPMAT_PNGH_GET(mat, i  , j)   ^= 0x0100ULL;
          SPMAT_PNGH_GET(mat, i+1, j)   ^= 0x8040ULL;
          break;
     case 7:
          SPMAT_PNGH_GET(mat, i-1, j)   ^= 0x0408000000000000;
          SPMAT_PNGH_GET(mat, i  , j)   ^= 0x0010000000000000;
          SPMAT_PNGH_GET(mat, i+1, j)   ^= 0x4020000000000000;
          SPMAT_PNGH_GET(mat, i-1, j+1) ^= 0x02ULL;
          SPMAT_PNGH_GET(mat, i  , j+1) ^= 0x01ULL;
          SPMAT_PNGH_GET(mat, i+1, j+1) ^= 0x80ULL;
          break;
     default:
          SPMAT_PNGH_GET(mat, i-1, j) ^= 0x020408ULL << off;
          SPMAT_PNGH_GET(mat, i,   j) ^= 0x010010ULL << off;
          SPMAT_PNGH_GET(mat, i+1, j) ^= 0x804020ULL << off;
          break;
     }
}


spmat spmat_read(char *file, int k)
{
     int M, N, magic;
     FILE *pf = fopen(file, "rb"); assert(pf);
     fread(&magic, sizeof(int), 1, pf);
     assert(magic == 666);
     fread(&M, sizeof(int), 1, pf);
     fread(&N, sizeof(int), 1, pf);
     uint8_t *pix = (uint8_t *) malloc(sizeof(uint8_t) * M*N);
     spmat mat = spmat_init(M, N, k);
     fread(pix, sizeof(uint8_t), M*N, pf);
     for (int i=0; i < M; i++)
          for (int j=0; j < N; j++) {
               if (pix[i*N + j]) {
                    if (i==0 || j==0 || i==M-1 || j== N-1) continue;
                    spmat_set(mat, i, j);
               }}

     free(pix);
     fclose(pf);
     return mat;
}


void spmat_write(char *file, spmat src)
{
     int magic = 666;
     FILE *pf = fopen(file, "wb"); assert(pf);
     fwrite(&magic, sizeof(int), 1, pf);
     fwrite(&(src.M), sizeof(int), 1, pf);
     fwrite(&(src.N), sizeof(int), 1, pf);
     for (int i = 0; i < src.M; i++)
          for (int j = 0; j < src.N; j++) {
               uint8_t pix = SPMAT_PIX_GET(src, i, j);
               fwrite(&pix, sizeof(uint8_t), 1, pf);
          }

     fclose(pf);
}
