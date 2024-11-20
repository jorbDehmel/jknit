'''
Provides a common interface for compilation drivers.
'''

import os
import subprocess


def compile_and_run(compiler_name: str) -> None:
    '''
    Uses the given compiler name to compile the source provided
    by argv[1] to an executable and runs it.
    '''

    name: str = f'{os.getpid()}.out'

    try:
        subprocess.call(compiler_name.split(' ') +
                        [os.sys.argv[1], '-o', name],
                        stdout=subprocess.DEVNULL)
        subprocess.call([f'./{name}'], stdout=os.sys.stdout)

    finally:
        os.remove(name)
