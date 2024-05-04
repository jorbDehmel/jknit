#!/usr/bin/python

'''
Driver for compiling Rust via `rustc`.
Jordan Dehmel, 2023-present
'''

import sys
import os


if __name__ == '__main__':

    assert len(sys.argv) > 1
    sys.exit(os.system('rustc -o a.out {sys.argv[1]} '
                       + '&& ./a.out && rm -f a.out'))
