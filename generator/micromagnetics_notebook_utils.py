#!/usr/bin/env python3

from ipywidgets import interact_manual, IntSlider, Text, Button
from IPython.display import display
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
    generator.do_it(x0Max - 1, x1Max - 1, outPath.encode("UTF-8"))
    print("XML generated in '{}'!".format(os.path.abspath(outPath)))

# Deploy to a POETS box.
def deploy(xmlPath="micromagnetics.xml", boxName="ayres"):
    print("Deploying micromagnetic application to POETS engine \"{}\"...".format(boxName))
    os.system("scp \"{}\" "
              "{}:~/repos/orchestrator-demo/application_staging/xml/"
              .format(xmlPath, boxName))
    print("Application deployed.")

# Run on POETS.
def run(boxName="ayres"):
    print("Running micromagnetic simulation on POETS engine \"{}\" via the Orchestrator..."
          .format(boxName))
    os.system("ssh {}-script -- \"bash run_demo.sh\"".format(boxName))
    print("Simulation complete.")

# Extract results.
def exfiltrate(boxName="ayres"):
    print("Exfiltrating simulation results from POETS engine \"{}\"..."
          .format(boxName))
    os.system("scp {}:~/repos/orchestrator-demo/bin/micromagnetics* ./"
              .format(boxName))
    print("Results exfiltrated.")

# Visualise
def visualise_results_3d():
    print("Rendering visualisation. Close the render window to continue.")
    postprocess.doit()

# Notebook shortcuts
def notebook_generator_slider():
    interact_manual.opts['manual_name'] = 'Generate'
    interact_manual(generate,
                    x0Max=IntSlider(min=1, max=51, step=1, value=15,
                                    description="x0 Points"),
                    x1Max=IntSlider(min=1, max=51, step=1, value=15,
                                    description="x1 Points"),
                    outPath=Text(value="micromagnetic_simulation.xml",
                                 description="Output file"))

def notebook_command_and_control():
    buttons = []
    buttons.append(Button(description="Deploy"))
    buttons[-1].on_click(lambda x: deploy())
    buttons.append(Button(description="Run"))
    buttons[-1].on_click(lambda x: run())
    buttons.append(Button(description="Exfiltrate"))
    buttons[-1].on_click(lambda x: exfiltrate())
    buttons.append(Button(description="Visualise"))
    buttons[-1].on_click(lambda x: visualise_results_3d())

    for button in buttons:
        display(button)

if __name__ == "__main__":
    print("This is a library designed to be imported by the generator and "
          "renderer notebooks. Do not load directly.")
