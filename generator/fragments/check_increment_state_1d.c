/* Now we need to check if our neighbours satisfy the update condition. If they
 * do, we update our state. */
uint8_t goingToUpdate;
float h_eff_dt_x0;
float h_eff_dt_x1;
float h_eff_dt_x2;
float m_x0_x0plus;
float m_x1_x0plus;
float m_x2_x0plus;
float m_x0_x0minus;
float m_x1_x0minus;
float m_x2_x0minus;
float dm_x0;
float dm_x1;
float dm_x2;
float m_dot_h_dt;
float target;
union {
    float f;
    uint32_t i;
} fisr;

goingToUpdate = 1;  /* Innocent until proven guilty. */
if (deviceProperties->x0plus_exists)
    if (deviceState->iteration_x0plus < deviceState->iteration)
        goingToUpdate = 0;
if (deviceProperties->x0minus_exists)
    if(deviceState->iteration_x0minus < deviceState->iteration)
        goingToUpdate = 0;
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
    }
    else
    {
        m_x0_x0plus = deviceState->m_x0_x0plus_even;
        m_x1_x0plus = deviceState->m_x1_x0plus_even;
        m_x2_x0plus = deviceState->m_x2_x0plus_even;
        m_x0_x0minus = deviceState->m_x0_x0minus_even;
        m_x1_x0minus = deviceState->m_x1_x0minus_even;
        m_x2_x0minus = deviceState->m_x2_x0minus_even;
    }

    /* Compute exchange field, multiplied by the time differential dt and other
     * scaling constants. */
    h_eff_dt_x0 = deviceProperties->exchange_coeff_dt *
        (m_x0_x0plus + m_x0_x0minus);
    h_eff_dt_x1 = deviceProperties->exchange_coeff_dt *
        (m_x1_x0plus + m_x1_x0minus);
    h_eff_dt_x2 = deviceProperties->exchange_coeff_dt *
        (m_x2_x0plus + m_x2_x0minus);

    /* Stir in a little DMI in a similar way (it's a cross product, and since
     * we're in 1d, there's no x1 differential and no x2 differential, so
     * there's no x0 contribution by DMI to the effective field). */
    h_eff_dt_x1 += deviceProperties->dmi_coeff_dt *
        (m_x2_x0plus - m_x2_x0minus);
    h_eff_dt_x2 += deviceProperties->dmi_coeff_dt *
        (m_x1_x0minus - m_x1_x0plus);

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
