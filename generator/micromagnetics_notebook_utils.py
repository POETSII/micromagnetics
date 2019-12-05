#!/usr/bin/env python3

import ctypes
import os

generatorLibPath = "./libgenerator.so"

# If you're importing this without the generator being built, we're going to
# complain.
if not os.path.exists(generatorLibPath):
    raise ImportError("Be sure to build the generator before importing this "
                      "library.")

# Grab the generator library.
generator = ctypes.CDLL(generatorLibPath)

# Expose the method elegantly.
def generate(x0Max, x1Max, outPath):
    if outPath == "":
        output = "unnamed_micromagnetic_simulation.xml"
    generator.do_it(x0Max, x1Max, outPath.encode("UTF-8"))
    print("XML generated! It is at '{}'".format(os.path.abspath(outPath)))

if __name__ == "__main__":
    print("This is a library designed to be imported by the generator and "
          "renderer notebooks. Do not load directly.")
