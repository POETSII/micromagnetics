#!/usr/bin/env python3

import os

# If you're importing this without the generator being built, we're going to
# complain.
if not os.path.exists("../generator"):
    raise ImportError("Import failed: Be sure to build the generator before "
                      "importing this library.")

def generate():
    pass


if __name__ == "__main__":
    print("This is a library designed to be imported by the generator and "
          "renderer notebooks. Do not load directly.")
