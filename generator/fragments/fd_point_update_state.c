/* We don't update our state before we burst our information, since we don't
 * know our neighbours' state yet. No indentation for sanity. */
if (DEVICESTATE(is_initialised) != 0) {

/* Effective field, multiplied by time differential in the x0, x1, and x2
 * directions. */
float h_eff_dt_x[3] = {0, 0, 0};

/* The magnetic moment field differential for this iteration, derived from
 * Euler updating, in the x0, x1, and x2 directions in the codomain. */
float dm_x[3];

float m_dot_h_dt;
float target;
union {
    float f;
    uint32_t i;
} fisr;    /* For fast-inverse square root. */

/* Which buffer should we use for neighbour's magnetisation? Odd or even? */
size_t odd_or_even = DEVICESTATE(iteration) & 1;

/* Compute exchange field, multiplied by the time differential dt and other
 * scaling constants. */
for (int m_dim = 0; m_dim < 3; m_dim++)
{
    for (int x_dim = 0; x_dim < {{v:dim}}; x_dim++)
    for (int ahead_or_behind = 0; ahead_or_behind < 2; ahead_or_behind++)
    {
        h_eff_dt_x[m_dim] += \
DEVICESTATE(m_x_neighbour)[odd_or_even][x_dim][ahead_or_behind][m_dim];
    }
    h_eff_dt_x[m_dim] *= DEVICEPROPERTIES(exchange_coeff_dt);
}

/* Stir in a little DMI in a similar way (it's a cross product).
 *
 * In 1d, there's no x1 differential and no x2 differential, so there's no x0
 * contribution by DMI to the effective field.
 *
 * In 2d, there's no x2 differential.
 *
 * NB: For 3D, this becomes (I'm pretty sure):
 *
 * h_eff_dt_x[0] += DEVICEPROPERTIES(dmi_coeff_dt) *
 *     (m_x2_x1plus - m_x2_x1minus - m_x1_x2plus + m_x1_x2minus);
 * h_eff_dt_x[1] += DEVICEPROPERTIES(dmi_coeff_dt) *
 *     (m_x0_x2plus - m_x0_x2minus + m_x2_x0plus - m_x2_x0minus);
 * h_eff_dt_x[2] += DEVICEPROPERTIES(dmi_coeff_dt) *
 *     (m_x1_x0minus - m_x1_x0plus - m_x0_x1plus + m_x0_x1minus);
 *
 * where:
 *
 * m_x0_x0plus = DEVICESTATE(m_x_neighbour)[odd_or_even][0][1][0];
 * m_x1_x0plus = DEVICESTATE(m_x_neighbour)[odd_or_even][0][1][1];
 * m_x2_x0plus = DEVICESTATE(m_x_neighbour)[odd_or_even][0][1][2];
 * m_x0_x0minus = DEVICESTATE(m_x_neighbour)[odd_or_even][0][0][0];
 * m_x1_x0minus = DEVICESTATE(m_x_neighbour)[odd_or_even][0][0][1];
 * m_x2_x0minus = DEVICESTATE(m_x_neighbour)[odd_or_even][0][0][2];
 * m_x0_x1plus = DEVICESTATE(m_x_neighbour)[odd_or_even][1][1][0];
 * m_x1_x1plus = DEVICESTATE(m_x_neighbour)[odd_or_even][1][1][1];
 * m_x2_x1plus = DEVICESTATE(m_x_neighbour)[odd_or_even][1][1][2];
 * m_x0_x1minus = DEVICESTATE(m_x_neighbour)[odd_or_even][1][0][0];
 * m_x1_x1minus = DEVICESTATE(m_x_neighbour)[odd_or_even][1][0][1];
 * m_x2_x1minus = DEVICESTATE(m_x_neighbour)[odd_or_even][1][0][2];
 * m_x0_x2plus = DEVICESTATE(m_x_neighbour)[odd_or_even][2][1][0];
 * m_x1_x2plus = DEVICESTATE(m_x_neighbour)[odd_or_even][2][1][1];
 * m_x2_x2plus = DEVICESTATE(m_x_neighbour)[odd_or_even][2][1][2];
 * m_x0_x2minus = DEVICESTATE(m_x_neighbour)[odd_or_even][2][0][0];
 * m_x1_x2minus = DEVICESTATE(m_x_neighbour)[odd_or_even][2][0][1];
 * m_x2_x2minus = DEVICESTATE(m_x_neighbour)[odd_or_even][2][0][2];
 */
#if {{v:dim}} == 1
h_eff_dt_x[1] += DEVICEPROPERTIES(dmi_coeff_dt) *
    (DEVICESTATE(m_x_neighbour)[odd_or_even][0][1][2] -
     DEVICESTATE(m_x_neighbour)[odd_or_even][0][0][2]);
h_eff_dt_x[2] += DEVICEPROPERTIES(dmi_coeff_dt) *
    (DEVICESTATE(m_x_neighbour)[odd_or_even][0][0][1] -
     DEVICESTATE(m_x_neighbour)[odd_or_even][0][1][1]);
#elif {{v:dim}} == 2
h_eff_dt_x[0] += DEVICEPROPERTIES(dmi_coeff_dt) *
    (DEVICESTATE(m_x_neighbour)[odd_or_even][1][1][2] -
     DEVICESTATE(m_x_neighbour)[odd_or_even][1][0][2]);
h_eff_dt_x[1] += DEVICEPROPERTIES(dmi_coeff_dt) *
    (DEVICESTATE(m_x_neighbour)[odd_or_even][0][1][2] -
     DEVICESTATE(m_x_neighbour)[odd_or_even][0][0][2]);
h_eff_dt_x[2] += DEVICEPROPERTIES(dmi_coeff_dt) *
    (DEVICESTATE(m_x_neighbour)[odd_or_even][0][0][1] -
     DEVICESTATE(m_x_neighbour)[odd_or_even][0][1][1] -
     DEVICESTATE(m_x_neighbour)[odd_or_even][1][1][0] +
     DEVICESTATE(m_x_neighbour)[odd_or_even][1][0][0]);
#elif {{v:dim}} == 3
h_eff_dt_x[0] += DEVICEPROPERTIES(dmi_coeff_dt) *
    (DEVICESTATE(m_x_neighbour)[odd_or_even][1][1][2] -
     DEVICESTATE(m_x_neighbour)[odd_or_even][1][0][2] -
     DEVICESTATE(m_x_neighbour)[odd_or_even][2][1][1] +
     DEVICESTATE(m_x_neighbour)[odd_or_even][2][0][1]);
h_eff_dt_x[1] += DEVICEPROPERTIES(dmi_coeff_dt) *
    (DEVICESTATE(m_x_neighbour)[odd_or_even][2][1][0] -
     DEVICESTATE(m_x_neighbour)[odd_or_even][2][0][0] +
     DEVICESTATE(m_x_neighbour)[odd_or_even][0][1][2] -
     DEVICESTATE(m_x_neighbour)[odd_or_even][0][0][2]);
h_eff_dt_x[2] += DEVICEPROPERTIES(dmi_coeff_dt) *
    (DEVICESTATE(m_x_neighbour)[odd_or_even][0][0][1] -
     DEVICESTATE(m_x_neighbour)[odd_or_even][0][1][1] -
     DEVICESTATE(m_x_neighbour)[odd_or_even][1][1][0] +
     DEVICESTATE(m_x_neighbour)[odd_or_even][1][0][0]);
#endif

/* Zeeman energy (influence of big external magnet). */
h_eff_dt_x[2] += DEVICEPROPERTIES(zeeman_coeff_dt);

/* Precessionless LLG, with Euler (see p30 of MLV's thesis) */
m_dot_h_dt = (DEVICESTATE(m_x)[0] * h_eff_dt_x[0]) +
    (DEVICESTATE(m_x)[1] * h_eff_dt_x[1]) +
    (DEVICESTATE(m_x)[2] * h_eff_dt_x[2]);

dm_x[0] = m_dot_h_dt * DEVICESTATE(m_x)[0] - h_eff_dt_x[0];
dm_x[1] = m_dot_h_dt * DEVICESTATE(m_x)[1] - h_eff_dt_x[1];
dm_x[2] = m_dot_h_dt * DEVICESTATE(m_x)[2] - h_eff_dt_x[2];

/* Step forward in time (Euler). Note that dt is incorporated into the
 * effective field coefficients (so make the FPU less confused, and the
 * compiler less stressed). */
DEVICESTATE(iteration) = DEVICESTATE(iteration) + 1;
for (int m_dim = 0; m_dim < 3; m_dim++)
    DEVICESTATE(m_x)[m_dim] = DEVICESTATE(m_x)[m_dim] + dm_x[m_dim];

{{f:normalise_fisr.c}}

}  /* See the iteration predicate at the top of this handler. */