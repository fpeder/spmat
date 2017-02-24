#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Usage: ./im2bmp.py <img> <out>

Options:
   -h --help   Show this message
"""

import struct

MAGIC = 666


def write(img, out):
    H, W = img.shape
    with open(out, 'wb') as f:
        f.write(struct.pack('1i', MAGIC))
        f.write(struct.pack('2i', H, W))
        for i in range(H):
            for j in range(W):
                f.write(struct.pack('1B', img[i, j]))
                

if __name__ == '__main__':
    from docopt import docopt
    from scipy.ndimage import imread

    args = docopt(__doc__)
    img = ~imread(args['<img>']) / 255
    assert(len(img) > 0)
    write(img[:, :, 0], args['<out>'])
