#!/usr/bin/env python3

import ctypes
import os
import sys
sys.path.append("../postprocessing/")
import postprocess

generatorLibPath = "./libgenerator.so"

# If you're importing this without the generator being built, we're going to
# complain.
if not os.path.exists(generatorLibPath):
    raise ImportError("Be sure to build the generator before importing this "
                      "library.")

# Grab the generator library.
generator = ctypes.CDLL(generatorLibPath)

# Expose the method elegantly.
def generate(x0Max, x1Max):
    print("Generating XML...")
    outPath = "micromagnetics.xml"
    generator.do_it(x0Max, x1Max, outPath.encode("UTF-8"))
    print("XML generated in '{}'!".format(os.path.abspath(outPath)))

# Deploy to a POETS box.
def deploy(xmlPath="micromagnetics.xml", boxName="ayres"):
    print("Deploying to POETS engine \"{}\"...".format(boxName))
    os.system("scp \"{}\" {}:~/repos/orchestrator/application_staging/xml/"
              .format(xmlPath, boxName))
    print("Application deployed.")

# Run on POETS.
def run(boxName="ayres"):
    os.system("ssh {}-script -- \"bash run_demo.sh\"")

# Extract results.
def exfiltrate(boxName="ayres"):
    print("Exfiltrating simulation results from POETS engine \"{}\"..."
          .format(boxName))
    os.system("scp {}:~/repos/orchestrator/bin/micromagnetics* ./"
              .format(boxName))
    print("Results exfiltrated.")

# Visualise
def run_my_shit():
    postprocess.doit()

if __name__ == "__main__":
    print("This is a library designed to be imported by the generator and "
          "renderer notebooks. Do not load directly.")
