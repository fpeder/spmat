#ifndef POINT_H
#define POINT_H

#include <stdio.h>

#define POINT_PACK(x, y)       (((x) << 16) | (y))
#define POINT_UNPACK(n, x, y)  y = (n) & 0x0000FFFF; x = (n) >> 16
#define POINT_PRINT(i, j)      printf("(%d,%d)", i, j)


void points_print(uint32_t *pt);


#endif /* POINT_H */
