#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Usage: ./datprint <img>

Options:
   -h --help  Show this message
"""

from __future__ import print_function
from dat2bmp import read


if __name__ == '__main__':
    import numpy as np
    from docopt import docopt
    
    args = docopt(__doc__)
    img = np.array(read(args['<img>']))
    for i in range(img.shape[0]):
        for j in range(img.shape[1]):
            print('%d' % img[i, j], end="")
        print()



