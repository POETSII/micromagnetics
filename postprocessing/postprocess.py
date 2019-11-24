#!/usr/bin/env python3
import numpy as np
from mayavi import mlab
import pandas as pd

def doit():
    df = pd.read_csv("micromagnetics.tsv", sep='\t',)

    # Figure out if this is a 1d, 2d, or 3d(!) plot. Pandas will shout if it
    # doesn't fit. Also precompute the size of the domain.
    dimensionality = len(df.columns) - 4
    if (dimensionality == 1):
        df.columns = ["x0", "iteration", "m0", "m1", "m2"]
        df = df.sort_values(by=["x0"])
        numberOfPoints = df["x0"].max() + 1

    elif (dimensionality == 2):
        df.columns = ["x0", "x1", "iteration", "m0", "m1", "m2"]
        df = df.sort_values(by=["x0", "x1"])
        numberOfPoints = (df["x0"].max() + 1) * (df["x1"].max() + 1)

    else:
        df.columns = ["x0", "x1", "x2", "iteration", "m0", "m1", "m2"]
        df = df.sort_values(by=["x0", "x1", "x2"])
        numberOfPoints = (df["x0"].max() + 1) *\
          (df["x1"].max() + 1) * (df["x2"].max() + 1)

    # "Normalise" by iteration count - the supervisor only accepts computation
    # every eight states.
    df["iteration"] = df["iteration"] // 8

    # Find the last iteration with all records in it - this is the final
    # complete iteration.
    iterationEnd = df["iteration"].max()
    while True:
        thisIteration = df.query("iteration == {}".format(iterationEnd))
        if (thisIteration.size == numberOfPoints * len(df.columns)):
            break
        iterationEnd -= 1

    # Rendering options
    mlab.options.offscreen = True
    backgroundColour = (0., 0., 0.)

    for iteration in [iterationEnd]:
    #for iteration in range(1, iterationEnd):
        thisIteration = df.query("iteration == {}".format(iteration))

        # Cone values
        m0 = thisIteration['m0'].values
        m1 = thisIteration['m1'].values
        m2 = thisIteration['m2'].values

        # Cone positions
        x0 = thisIteration['x0'].values
        if (dimensionality < 3):
            x2 = np.zeros_like(x0)
        else:
            x2 = thisIteration['x2'].values

        if (dimensionality < 2):
            x1 = np.zeros_like(x0)
        else:
            x1 = thisIteration['x1'].values

        # I know it's bad to set up and tear down the visualisation each
        # iteration, but none of the examples for surfaces work, and there is
        # no documentation on vector_scatter sources (*sigh*)
        mlab.figure(bgcolor=backgroundColour)

        # Polydata source
        source = mlab.pipeline.vector_scatter(x0, x1, x2, m0, m1, m2)

        # Colour by z-component value.
        zFilter = mlab.pipeline.extract_vector_components(source)
        zFilter.component = 'z-component'

        # Cones and cone setup
        vectors = mlab.pipeline.vectors(zFilter,
                                        colormap="PuOr",
                                        mode="cone",
                                        resolution=128)
        vectors.glyph.glyph.color_mode = 'color_by_scalar'
        vectors.actor.mapper.scalar_range = np.array([-1., 1.])
        vectors.glyph.glyph_source.glyph_source.center = np.array([0., 0., 0.])
        vectors.glyph.glyph_source.glyph_source.radius = 0.4

        # Display
        #mlab.show()
        #mlab.savefig("out_{0:06d}.png".format(iteration), magnification=4)
        mlab.savefig("out.png", magnification=4)

        # Close the figure we've opened.
        mlab.close()

if __name__ == "__main__":
    doit()
