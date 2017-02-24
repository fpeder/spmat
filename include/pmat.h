#ifndef PMAT_H
#define PMAT_H

#include <stdint.h>


typedef struct {
     int M, N, sz;
     uint64_t *data;
} pmat;


pmat pmat_init(int M, int N, int len);
void pmat_free(pmat mat);


#endif /* PMAT_H */

