#ifndef MORPH_H
#define MORPH_H

#include "spmat.h"


void spmat_erode(spmat in, spmat ou, int k);
void spmat_dilate(spmat in, spmat ou, int k);


#endif /* MORPH_H */
