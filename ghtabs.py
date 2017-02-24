#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
from itertools import product


def del_cond(n, p8, p7, p6, p5, p4, p3, p2, p1):
    C = ((~p2 & (p3 | p4)) + (~p4 & (p5 | p6)) +
         (~p6 & (p7 | p8)) + (~p8 & (p1 | p2)))
    N1 = (p1 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8)
    N2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p1)
    N = N1 if N1 < N2 else N2
    m = (p2 | p3 | ~p5) & p4 if n == 1 else (p6 | p7 | ~p1) & p8
    return C == 1 and N >= 2 and N <= 3 and m == 0


def gen_lutab(n):
    tab = np.empty(256, dtype=np.uint8)
    idx = 0;
    for p in product([0, 1], repeat=8):
        tab[idx] = del_cond(n, *p)
        idx += 1
    return tab


asd = gen_lutab(0)



if __name__ == '__main__':
    with open('r.tab', 'wb') as f:
        f.write(gen_lutab(0).tobytes('C'))

    with open('l.tab', 'wb') as f:
        f.write(gen_lutab(1).tobytes('C'))


