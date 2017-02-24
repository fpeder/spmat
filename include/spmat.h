#ifndef SPMAT_H
#define SPMAT_H

#include "pmat.h"

#define CLOCKWISE

#define NL printf("\n")
#define SP printf(" ")


#define SPMAT_PPIX(m, i, j)\
     m.pix.data[(i) * m.pix.N + ((j) >> 6)]

#define SPMAT_PIX_SET(m, i, j) \
     SPMAT_PPIX(m, i, j) |= 1ULL << ((j) & 63)

#define SPMAT_PIX_DEL(m, i, j) \
     SPMAT_PPIX(m, i, j) ^= 1ULL << ((j) & 63)

#define SPMAT_PIX_GET(m, i, j) \
     (SPMAT_PPIX(m, i, j) >> (((j) & 63)) & 1ULL)

#define SPMAT_PNGH_GET(m, i, j) \
     m.ngh.data[(i) * m.ngh.N + ((j) >> 3)]

#define SPMAT_NGH_GET(m, i, j) \
     (uint8_t)(SPMAT_PNGH_GET(m, i, j) >> (((j) & 7) << 3))


typedef struct {
     int M, N, k;
     pmat pix, ngh;
} spmat;


spmat spmat_init    (int M, int N, int k);
spmat spmat_alloc_copy    (spmat mat);
void  spmat_copy   (spmat src, spmat dst);
void  spmat_free    (spmat mat);
spmat spmat_read    (char *file, int k);
void  spmat_write   (char *file, spmat mat);
void  spmat_set     (spmat mat, int i, int j);
void  spmat_del     (spmat mat, int i, int j);
spmat spmat_read    (char *file, int k);
void  spmat_first   (spmat mat, int *i, int *k);
int   spmat_foreach (spmat mat, int *ii, int *jj);

#endif /* SPMAT_H */
