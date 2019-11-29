/* Now we need to check if our neighbours satisfy the update condition. If they
 * do, we update our state. */

uint8_t goingToUpdate;  /* 1 if our neighbours' iteration is >= ours. */

float h_eff_dt_x0;  /* Effective field, multiplied by time differential... */
float h_eff_dt_x1;  /* ...in the x0, x1, and x2 directions. Signage is... */
float h_eff_dt_x2;  /* ...questionable. */

float m_x0_x0plus;  /* The x0 (or x1 or x2) -component of the magnetic... */
float m_x1_x0plus;  /* ...moment field value at the neighbour one ahead... */
float m_x2_x0plus;  /* ...in the x0 direction in the domain. */

float m_x0_x0minus;  /* As above, but for one behind in the x0 direction. */
float m_x1_x0minus;
float m_x2_x0minus;

float m_x0_x1plus;  /* As above, but for one ahead in the x1 direction. */
float m_x1_x1plus;
float m_x2_x1plus;

float m_x0_x1minus;  /* As above, but for one behind in the x1 direction. */
float m_x1_x1minus;
float m_x2_x1minus;

float dm_x0;  /* The magnetic moment field differential for this... */
float dm_x1;  /* iteration, derived from Euler updating, in the x0, x1, and */
float dm_x2;  /* x2 directions in the codomain.

float m_dot_h_dt;
float target;
union {
    float f;
    uint32_t i;
} fisr;  /* For fast-inverse square root. */

goingToUpdate = 1;  /* Innocent until proven guilty. */
if (deviceState->iteration_x0plus < deviceState->iteration) goingToUpdate = 0;
if (deviceState->iteration_x0minus < deviceState->iteration) goingToUpdate = 0;
if (deviceState->iteration_x1plus < deviceState->iteration) goingToUpdate = 0;
if (deviceState->iteration_x1minus < deviceState->iteration) goingToUpdate = 0;
if (goingToUpdate == 1)
{
    /* Which buffer should we use for neighbour's magnetisation? */
    if (deviceState->iteration & 1)
    {
        m_x0_x0plus = deviceState->m_x0_x0plus_odd;
        m_x1_x0plus = deviceState->m_x1_x0plus_odd;
        m_x2_x0plus = deviceState->m_x2_x0plus_odd;
        m_x0_x0minus = deviceState->m_x0_x0minus_odd;
        m_x1_x0minus = deviceState->m_x1_x0minus_odd;
        m_x2_x0minus = deviceState->m_x2_x0minus_odd;
        m_x0_x1plus = deviceState->m_x0_x1plus_odd;
        m_x1_x1plus = deviceState->m_x1_x1plus_odd;
        m_x2_x1plus = deviceState->m_x2_x1plus_odd;
        m_x0_x1minus = deviceState->m_x0_x1minus_odd;
        m_x1_x1minus = deviceState->m_x1_x1minus_odd;
        m_x2_x1minus = deviceState->m_x2_x1minus_odd;
    }
    else
    {
        m_x0_x0plus = deviceState->m_x0_x0plus_even;
        m_x1_x0plus = deviceState->m_x1_x0plus_even;
        m_x2_x0plus = deviceState->m_x2_x0plus_even;
        m_x0_x0minus = deviceState->m_x0_x0minus_even;
        m_x1_x0minus = deviceState->m_x1_x0minus_even;
        m_x2_x0minus = deviceState->m_x2_x0minus_even;
        m_x0_x1plus = deviceState->m_x0_x1plus_even;
        m_x1_x1plus = deviceState->m_x1_x1plus_even;
        m_x2_x1plus = deviceState->m_x2_x1plus_even;
        m_x0_x1minus = deviceState->m_x0_x1minus_even;
        m_x1_x1minus = deviceState->m_x1_x1minus_even;
        m_x2_x1minus = deviceState->m_x2_x1minus_even;
    }

    /* Compute exchange field, multiplied by the time differential dt and other
     * scaling constants. */
    h_eff_dt_x0 = deviceProperties->exchange_coeff_dt *
        (m_x0_x0plus + m_x0_x0minus + m_x0_x1plus + m_x0_x1minus);
    h_eff_dt_x1 = deviceProperties->exchange_coeff_dt *
        (m_x1_x0plus + m_x1_x0minus + m_x1_x1plus + m_x1_x1minus);
    h_eff_dt_x2 = deviceProperties->exchange_coeff_dt *
        (m_x2_x0plus + m_x2_x0minus + m_x2_x1plus + m_x2_x1minus);

    /* Stir in a little DMI in a similar way (it's a cross product, and since
     * we're in 2d, there's no x2 differential).
     *
     * NB: For 3D, this becomes (I think):
     *
     * h_eff_dt_x0 += deviceProperties->dmi_coeff_dt *
     *     (m_x2_x1plus - m_x2_x1minus - m_x1_x2plus + m_x1_x2minus);
     * h_eff_dt_x1 += deviceProperties->dmi_coeff_dt *
     *     (m_x0_x2plus - m_x0_x2minus + m_x2_x0plus - m_x2_x0minus);
     * h_eff_dt_x2 += deviceProperties->dmi_coeff_dt *
     *     (m_x1_x0minus - m_x1_x0plus - m_x0_x1plus + m_x0_x1minus);
     */
    h_eff_dt_x0 += deviceProperties->dmi_coeff_dt *
        (m_x2_x1plus - m_x2_x1minus);
    h_eff_dt_x1 += deviceProperties->dmi_coeff_dt *
        (m_x2_x0plus - m_x2_x0minus);
    h_eff_dt_x2 += deviceProperties->dmi_coeff_dt *
        (m_x1_x0minus - m_x1_x0plus - m_x0_x1plus + m_x0_x1minus);

    /* Zeeman energy (influence of big external magnet). */
    h_eff_dt_x2 += deviceProperties->zeeman_coeff_dt;

    /* Precessionless LLG, with Euler (see p30 of MLV's thesis) */
    m_dot_h_dt = (deviceState->m_x0 * h_eff_dt_x0) +
        (deviceState->m_x1 * h_eff_dt_x1) +
        (deviceState->m_x2 * h_eff_dt_x2);

    dm_x0 = m_dot_h_dt * deviceState->m_x0 - h_eff_dt_x0;
    dm_x1 = m_dot_h_dt * deviceState->m_x1 - h_eff_dt_x1;
    dm_x2 = m_dot_h_dt * deviceState->m_x2 - h_eff_dt_x2;

    /* Step forward in time (Euler). Note that dt is incorporated into the
     * effective field coefficients (so make the FPU less confused). */
    deviceState->iteration = deviceState->iteration + 1;
    deviceState->m_x0 = deviceState->m_x0 + dm_x0;
    deviceState->m_x1 = deviceState->m_x1 + dm_x1;
    deviceState->m_x2 = deviceState->m_x2 + dm_x2;

    /* Normalise using Fast Inverse Square Root. Never thought I'd need this
     * again! Two Newton-Raphson iterations gets us to e-6 accuracy at
     * |m|=1. */
    target = (deviceState->m_x0 * deviceState->m_x0) +
        (deviceState->m_x1 * deviceState->m_x1) +
        (deviceState->m_x2 * deviceState->m_x2);
    fisr = {target};
    fisr.i = 0x5f3759df - ( fisr.i >> 1 );
    fisr.f *= (1.5 - ( target * 0.5 * fisr.f * fisr.f));  // First NR
    fisr.f *= (1.5 - ( target * 0.5 * fisr.f * fisr.f));  // Second NR
    deviceState->m_x0 *= fisr.f;
    deviceState->m_x1 *= fisr.f;
    deviceState->m_x2 *= fisr.f;

    /* We did it! This variable notifies the ReadyToSend (RTS) handler that
     * we've just updated. Once read, the output pin handler will set this to
     * zero, so that successive RTSs don't send a message unless a new state
     * can be computed. */
    deviceState->did_we_just_update = 1;
}
