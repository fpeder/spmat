#include "thin.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static int iter=1;

static const
uint8_t rtab[] = {0,0,0,0,0,0,0,1, 0,0,1,1,0,0,1,1,
                  0,0,0,0,0,0,0,0, 0,0,1,1,1,0,1,1,
                  0,0,0,0,0,0,0,0, 1,0,1,1,1,0,1,1,
                  0,0,0,0,0,0,0,0, 1,0,1,1,1,0,1,1,
                  
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0, 1,0,1,0,1,0,1,0,
                  1,0,0,0,0,0,0,0, 1,0,1,0,1,0,1,0,
                  
                  0,0,0,1,0,0,0,1, 0,0,0,1,0,0,0,1,
                  0,0,0,0,0,0,0,0, 0,0,0,1,0,0,0,1,
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                  
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0};

static const
uint8_t ltab[] = {0,0,0,0,0,0,0,1, 0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,
                  
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                  1,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,
                  
                  0,0,1,1,0,0,1,1, 0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                  1,1,1,1,0,0,1,1, 0,0,0,0,0,0,0,0,
                  1,1,1,1,0,0,0,0, 1,1,0,0,0,0,0,0,
                  
                  0,1,1,1,0,0,1,1, 0,0,0,0,0,0,0,0,
                  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
                  1,1,1,1,0,0,1,1, 0,0,0,0,0,0,0,0,
                  1,1,1,1,0,0,0,0, 1,1,0,0,0,0,0,0};

static const
uint64_t prtab[] = {15924971278142852224ULL,
                    6125270426688946176ULL,
                    2281736328ULL,
                    0ULL};
static const
uint64_t pltab[] = {72057594037928064ULL,
                    72339069014638592ULL,
                    220395795822674124ULL,
                    220395795822674126ULL};



static
uint8_t gh_del(uint8_t ngh)
{
     uint8_t p1, p2, p3, p4, p5, p6, p7, p8;
     uint8_t C, N1, N2, N, d;

     p1 = (ngh >> 7) & 1;
     p2 = (ngh >> 6) & 1;
     p3 = (ngh >> 5) & 1;
     p4 = (ngh >> 4) & 1;
     p5 = (ngh >> 3) & 1;
     p6 = (ngh >> 2) & 1;
     p7 = (ngh >> 1) & 1;
     p8 = ngh        & 1;

     C = (((!p2) && (p3 || p4)) + ((!p4) && (p5 || p6)) +
          ((!p6) && (p7 || p8)) + ((!p8) && (p1 || p2)));

     N1 = (p1 || p2) + (p3 || p4) + (p5 || p6) + (p7 || p8);
     N2 = (p2 || p3) + (p4 || p5) + (p6 || p7) + (p8 || p1);

     N = N1 < N2 ? N1 : N2;
     d = iter ? ((p2 || p3 || (!p5)) && p4) :
                ((p6 || p7 || (!p1)) && p8);

     return (C == 1) && (N >= 2) && (N <= 3) && (d == 0);
}


int gh_iter_nolookup(spmat src, spmat dst)
{
     uint8_t *in = (uint8_t *) src.ngh.data;
     uint8_t ngh, pix; int dels=0;

     for (int i=1; i < src.M - 1; i++) {
          for (int j=1; j < src.N - 1; j++) {
               pix = in[i*src.N + j+1] & 1;
               ngh = in[i*src.N + j];
               if (pix && gh_del(ngh)) {
                    spmat_del(dst, i, j);
                    dels++;
               }}}

     iter ^= 1;
     return dels;
}


int gh_iter_lookup(spmat src, spmat dst)
{
     const uint8_t *tab = iter ? rtab : ltab;
     uint8_t *in = (uint8_t *) src.ngh.data;
     uint8_t pix, ngh; int dels=0;

     for (int i=1; i < src.M - 1; i++) {
          for (int j=1; j < src.N - 1; j++) {
               pix = in[i*src.N + j+1] & 1;
               ngh = in[i*src.N + j];
               if (pix && tab[ngh]) {
                    spmat_del(dst, i, j);
                    dels++;
               }}}

     iter ^= 1;
     return dels;
}


int gh_iter_plookup(spmat src, spmat dst)
{
     uint8_t *in = (uint8_t *) src.ngh.data;
     const uint64_t *tab = iter ? prtab : pltab;
     uint8_t pix, ngh, idx, k;
     int dels=0;

     for (int i=1; i < src.M - 1; i++) {
          for (int j=1; j < src.N - 1; j++) {
               pix = in[i*src.N + j+1] & 1;
               ngh = in[i*src.N + j];
               idx = ngh >> 6; k = ngh & 63;
               if (pix && ((tab[idx] >> k) & 1)){
                    spmat_del(dst, i, j);
                    dels++;
               }}}

     iter ^= 1;
     return dels;
}


void spmat_guohall(spmat src, spmat dst,
                    int (*caz)(spmat a, spmat b))
{
     iter = 1; int dels=0;
     spmat tmp = spmat_alloc_copy(src);
     do {
          dels = caz(tmp, dst);
          spmat_copy(dst, tmp);
     } while (dels);

     spmat_free(tmp);
}



