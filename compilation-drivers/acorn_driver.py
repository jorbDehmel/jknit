#!/usr/bin/python

'''
Driver for compiling Oak code via `acorn`.
Jordan Dehmel, 2024-present
'''

import sys
import os


if __name__ == '__main__':

    assert len(sys.argv) > 1
    sys.exit(os.system(f'acorn -oE a.out {sys.argv[1]} && rm -f a.out'))
