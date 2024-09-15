#!/usr/bin/python

'''
Driver for compiling C++ code via `clang++`.
Jordan Dehmel, 2023-present
'''

import sys
import os


if __name__ == '__main__':
    try:
        result: int = os.system(
            f'clang++ {sys.argv[1]} -o a.out > /dev/null')

        assert result == 0
        result = os.system('./a.out')
        assert result == 0
    finally:
        os.system('rm a.out')
