#!/usr/bin/env python3
import numpy as np
from mayavi import mlab
import pandas as pd


def load_data(dataPath):
    df = pd.read_csv(dataPath)

    # Figure out if this is a 1d, 2d, or 3d(!) plot. Pandas will shout if it
    # doesn't fit. Also precompute the size of the domain.
    dimensionality = get_data_dimensionality(df)
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
    return df


def get_data_dimensionality(df):
    return len(df.columns) - 4


def get_maximum_iteration_from_data(df):
    # Compute the number of points in the data.
    dimensionality = get_data_dimensionality(df)
    if (dimensionality == 1):
        numberOfPoints = df["x0"].max() + 1
    elif (dimensionality == 2):
        numberOfPoints = (df["x0"].max() + 1) * (df["x1"].max() + 1)
    else:
        numberOfPoints = (df["x0"].max() + 1) *\
          (df["x1"].max() + 1) * (df["x2"].max() + 1)

    # Find the last iteration with all records in it - this is the final
    # complete iteration.
    iterationEnd = df["iteration"].max()
    while True:
        thisIteration = df.query("iteration == {}".format(iterationEnd))
        if (thisIteration.size == numberOfPoints * len(df.columns)):
            break
        iterationEnd -= 1
    return iterationEnd


def doit(show=True):
    df = load_data("micromagnetics.csv")
    iterationEnd = get_maximum_iteration_from_data(df)
    dimensionality = get_data_dimensionality(df)

    # Rendering options
    backgroundColour = (0., 0., 0.)
    if show is False:
        mlab.options.offscreen = True

    # Set up visualisation, from the top.
    thisIteration = df.query("iteration == 1")

    # Cone values (recomputed in loop)
    m0 = thisIteration['m0'].values
    m1 = thisIteration['m1'].values
    m2 = thisIteration['m2'].values

    # Cone positions (recomputed in loop)
    x0 = thisIteration['x0'].values
    if (dimensionality < 3):
        x2 = np.zeros_like(x0)
    else:
        x2 = thisIteration['x2'].values

    if (dimensionality < 2):
        x1 = np.zeros_like(x0)
    else:
        x1 = thisIteration['x1'].values

    # Compulsory faffing
    mlab.figure(bgcolor=backgroundColour)

    # Polydata source
    origSource = mlab.pipeline.vector_scatter(x0, x1, x2, m0, m1, m2)

    # Colour by z-component value.
    zFilter = mlab.pipeline.extract_vector_components(origSource)
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

    print("Rendering major iterations {}-{}...".format(1, 9))
    # for iteration in range(1, iterationEnd):
    for iteration in [iterationEnd]:
        thisIteration = df.query("iteration == {}".format(iteration))
        if (iteration % 10 == 0):
            print("Rendering major iterations {}-{}..."
                  .format(iteration, iteration + 9))

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

        # Update data structures.
        vectors.mlab_source.reset(x=x0, y=x1, z=x2,
                                  u=m0, v=m1, w=m2)

        # Save (or show)
        if show is False:
            mlab.savefig("out_{0:06d}.png".format(iteration), magnification=4)
        else:
            mlab.show()

    if show is False:
        mlab.close()


if __name__ == "__main__":
    doit(show=True)
