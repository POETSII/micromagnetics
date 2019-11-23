#!/usr/bin/env python3
import numpy as np
from mayavi import mlab
import pandas as pd

def doit():
    df = pd.read_csv("micromagnetics_1d_20_data.tsv", sep='\t',
                     names=["x0", "iteration", "m0", "m1", "m2"])
    lastIteration = df.query('iteration == 2128').sort_values(by=["x0"])
    x0 = lastIteration['x0'].values
    x1 = np.zeros_like(x0)
    x2 = np.zeros_like(x0)
    m0 = lastIteration['m0'].values
    m1 = lastIteration['m1'].values
    m2 = lastIteration['m2'].values

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

    mlab.show()
    # Wanna save?
    # mlab.savefig("out.png", magnification=4)

if __name__ == "__main__":
    doit()
