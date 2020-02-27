#include "initial_conditions.h"

/* Skyrmion profile generator. For a given co-ordinate in the space, returns
 * the magnetisation at that co-ordinate given that a skyrmion exists centred
 * at x0=x1=x2=0. If the co-ordinate is beyond the radius of the skyrmion, a
 * ferromagnetic solution is returned.
 *
 * It's a right-handed Bloch skyrmion.
 *
 * Arguments:
 *
 *  - mx0/1/2: Pointer to a float to write to with the x0/1/2 component of
 *    data. Note that the skyrmion is invariant in x2.
 *
 *  - x0/1/2: Co-ordinate to compute the profile for.
 *
 *  - radius: Radius at which a ferromagnetic solution is returned.
 *
 * Returns 1 if the co-ordinate is within the skyrmion, and 0 otherwise (this
 * can be exploited to produce skyrmion lattices more easily). */
unsigned skyrmion_profile(float* mx0, float* mx1, float* mx2,
                          float x0, float x1, float x2,
                          float radius)
{
    float radialCoordinate;
    float argumentCoordinate;
    float ma;
    float norm;

    /* Compute the radial co-ordinate (to go with the argument co-ordinate
     * later). */
    radialCoordinate = sqrt(pow(x0, 2) + pow(x1, 2));

    /* Determine if the input co-ordinate is within the skyrmion. If not, exit
     * quickly. */
    if (radialCoordinate > radius)
    {
        *mx0 = 0;
        *mx1 = 0;
        *mx2 = 1;
        return 0;
    }

    /* Compute the argument co-ordinate. */
    if (fabs(x0) < 1e-6)
    {
        if (fabs(x1) < 1e-6)
            argumentCoordinate = 0;
        else if (x1 > 0)
            argumentCoordinate = PI / 2;
        else
            argumentCoordinate = -PI / 2;
    }
    else
        argumentCoordinate = atan2(x1, x0);

    /* Define some magnetisation components in cylindrical co-ordinates. */
    *mx2 = -cos(PI * radialCoordinate / radius);
    ma = sin(PI * radialCoordinate / radius);

    /* Convert to Cartesian form */
    *mx0 = -sin(argumentCoordinate) * ma;
    *mx1 = cos(argumentCoordinate) * ma;

    /* Normalise. */
    norm = sqrt(pow(*mx0, 2) + pow(*mx1, 2) + pow(*mx2, 2));
    *mx0 /= norm;
    *mx1 /= norm;
    *mx2 /= norm;
    return 1;
}
