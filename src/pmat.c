#include "pmat.h"

#include <stdlib.h>
#include <assert.h>


pmat pmat_init(int M, int N, int len)
{
     int pN = N % len ? N/len + 1 : N/len;
     pmat mat = {M, pN,  M*pN, NULL};
     mat.data = (uint64_t *) calloc(sizeof(uint64_t), mat.sz);
     assert(mat.data);
     return mat;
}



void pmat_free(pmat mat)
{
     free(mat.data);
}
