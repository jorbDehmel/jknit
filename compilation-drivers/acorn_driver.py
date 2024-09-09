#!/usr/bin/python

'''
Driver for compiling Oak code via `acorn`.
Jordan Dehmel, 2024-present
'''

import sys
import os


if __name__ == '__main__':
    result: int = os.system(f'acorn -MEe {sys.argv[1]}')
    assert result == 0
