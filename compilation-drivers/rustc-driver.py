#!/usr/bin/python
import sys
import os


if __name__ == "__main__":
    if len(sys.argv) > 1:
        os.system("rustc -o output_temp.out " + sys.argv[1] + " && ./output_temp.out && rm -f output_temp.out")
    else:
        raise "Error: No argument passed"
