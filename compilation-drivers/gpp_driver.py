#!/usr/bin/python

'''
Driver for compiling C++ code via `g++`.
Jordan Dehmel, 2023-present
'''

import sys
import os


if __name__ == '__main__':

    assert len(sys.argv) > 1
    sys.exit(os.system(f'g++ -o output_temp.out {sys.argv[1]} '
                       + '&& ./output_temp.out && rm -f output_temp.out'))
