#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Usage: ./dat2bmp.py <img> [<out>]

Options:
   -h --help   Show this message
"""

import struct

MAGIC = 666


def read(src):
    bmp = []
    with open(src, 'rb') as f:
        magic = struct.unpack('1i', f.read(4))[0]
        assert magic == MAGIC
        H, W = struct.unpack('2i', f.read(8))
        for i in range(H):
            row = struct.unpack(str(W)+'B', f.read(W))
            bmp.append(row)
    return bmp


if __name__ == '__main__':
    import numpy as np
    import matplotlib.pyplot as plt
    from docopt import docopt
    from scipy.misc import imsave

    args = docopt(__doc__)
    img = read(args['<img>'])
    if args['<out>']:
        img = ~(np.array(img, np.uint8) * 255)
        img = np.tile(np.expand_dims(img, axis=2), (1, 1, 3))
        imsave(args['<out>'], img)
    else:
        plt.imshow(img, interpolation='nearest', cmap='Greys')
        plt.axis('off')
        plt.show()
