#!/usr/bin/python

'''
Driver for creating `png` files from `.dot` or `.gv` files.
Jordan Dehmel, 2023-present
'''

import sys
import os


if __name__ == '__main__':
    # Prevent collisions
    name: str = 'jknit_figure'
    num: int = 1
    while os.path.exists(f'{name}_{num}.png'):
        num += 1

    # Create output file
    result: int = os.system(
        f'dot {sys.argv[1]} -Tpng -o {name}_{num}.png')

    assert result == 0

    # Identify where to find it
    print(f'Saved graphviz figure to {name}_{num}.png')
