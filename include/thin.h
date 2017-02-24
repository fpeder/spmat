#ifndef THIN_H
#define THIN_H

#include "spmat.h"



int gh_iter_nolookup(spmat src, spmat dst);
int gh_iter_lookup(spmat src, spmat dst);
int gh_iter_plookup(spmat src, spmat dst);

void spmat_guohall(spmat src, spmat dst,
                   int (*iter)(spmat a, spmat b));


#endif /* THIN_H */
