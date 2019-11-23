#!/usr/bin/env python3
import numpy as np
from mayavi import mlab
import pandas as pd

def doit():
    df = pd.read_csv("micromagnetics.tsv", sep='\t',
                     names=["x0", "iteration", "m0", "m1", "m2"])
    df["iteration"] = df["iteration"] // 8
    df = df.sort_values(by=["x0"])

    # Find the last iteration with all records in it.
    iterationEnd = df["iteration"].max()
    while True:
        thisIteration = df.query("iteration == {}".format(iterationEnd))
        if (thisIteration.size == 101 * 5):  # 5 columns
            break
        iterationEnd -= 1

    # I know it's bad to set up and tear down the visualisation each iteration,
    # but none of the examples for surfaces work, and there is no documentation
    # on vector_scatter sources (*sigh*)
    mlab.options.offscreen = True

    #for iteration in [iterationEnd]:
    for iteration in range(1, iterationEnd):
        thisIteration = df.query("iteration == {}".format(iteration))

        x0 = thisIteration['x0'].values
        x1 = np.zeros_like(x0)
        x2 = np.zeros_like(x0)
        m0 = thisIteration['m0'].values
        m1 = thisIteration['m1'].values
        m2 = thisIteration['m2'].values

        mlab.figure(bgcolor=(0., 0., 0.))
        source = mlab.pipeline.vector_scatter(x0, x1, x2, m0, m1, m2)
        zFilter = mlab.pipeline.extract_vector_components(source)
        zFilter.component = 'z-component'
        vectors = mlab.pipeline.vectors(zFilter,
                                        colormap="PuOr",
                                        mode="cone",
                                        resolution=128)
        vectors.glyph.glyph.color_mode = 'color_by_scalar'
        vectors.actor.mapper.scalar_range = np.array([-1., 1.])
        vectors.glyph.glyph_source.glyph_source.center = np.array([0., 0., 0.])
        vectors.glyph.glyph_source.glyph_source.radius = 0.4

        # Wanna show?
        #mlab.show()
        # Wanna save?
        mlab.savefig("out_{0:06d}.png".format(iteration), magnification=4)
        #mlab.savefig("out.png", magnification=4)
        mlab.close()

if __name__ == "__main__":
    doit()
