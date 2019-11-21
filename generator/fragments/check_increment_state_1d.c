/* Now we need to check if our neighbours satisfy the update condition. If they
 * do, we update our state. */
deviceState->goingToUpdate = 1;  /* Innocent until proven guilty. */
if (deviceProperties->x0plus_exists)
    if (deviceState->iteration_x0plus < deviceState->iteration)
        deviceState->goingToUpdate = 0;
if (deviceProperties->x0minus_exists)
    if(deviceState->iteration_x0minus < deviceState->iteration)
        deviceState->goingToUpdate = 0;
if (deviceState->goingToUpdate == 1)
{
    /* Which buffer should we use for neighbour's magnetisation? */
    if (deviceState->iteration & 1)
    {
        deviceState->m_x0_x0plus = deviceState->m_x0_x0plus_odd;
        deviceState->m_x1_x0plus = deviceState->m_x1_x0plus_odd;
        deviceState->m_x2_x0plus = deviceState->m_x2_x0plus_odd;
        deviceState->m_x0_x0minus = deviceState->m_x0_x0minus_odd;
        deviceState->m_x1_x0minus = deviceState->m_x1_x0minus_odd;
        deviceState->m_x2_x0minus = deviceState->m_x2_x0minus_odd;
    }
    else
    {
        deviceState->m_x0_x0plus = deviceState->m_x0_x0plus_even;
        deviceState->m_x1_x0plus = deviceState->m_x1_x0plus_even;
        deviceState->m_x2_x0plus = deviceState->m_x2_x0plus_even;
        deviceState->m_x0_x0minus = deviceState->m_x0_x0minus_even;
        deviceState->m_x1_x0minus = deviceState->m_x1_x0minus_even;
        deviceState->m_x2_x0minus = deviceState->m_x2_x0minus_even;
    }

    /* Compute exchange field, multiplied by the time differential dt */
    deviceState->h_eff_dt_x0 = deviceProperties->exchange_coeff_dt *
        (deviceState->m_x0_x0plus + deviceState->m_x0_x0minus);
    deviceState->h_eff_dt_x1 = deviceProperties->exchange_coeff_dt *
        (deviceState->m_x1_x0plus + deviceState->m_x1_x0minus);
    deviceState->h_eff_dt_x2 = deviceProperties->exchange_coeff_dt *
        (deviceState->m_x2_x0plus + deviceState->m_x2_x0minus);

    /* Stir in a little DMI (it's a cross product, and since we're in 1d,
     * there's no x1 differential and no x2 differential, so there's no x0
     * contribution by DMI to the effective field). */
    deviceState->h_eff_dt_x1 += deviceProperties->dmi_coeff_dt *
        (deviceState->m_x2_x0plus - deviceState->m_x2_x0minus);
    deviceState->h_eff_dt_x2 += deviceProperties->dmi_coeff_dt *
        (deviceState->m_x1_x0minus - deviceState->m_x1_x0plus);

    /* Zeeman energy (influence of big external magnet). */
    deviceState->h_eff_dt_x2 += deviceProperties->zeeman_coeff_dt;

    /* Precessionless LLG, with Euler (see p30 of MLV's thesis) */
    deviceState->m_dot_h_dt = deviceState->m_x0 * deviceState->h_eff_dt_x0 +
        deviceState->m_x1 * deviceState->h_eff_dt_x1 +
        deviceState->m_x2 * deviceState->h_eff_dt_x2;
    deviceState->dm_x0 = deviceState->m_dot_h_dt * deviceState->m_x0 -
        deviceState->h_eff_dt_x0;
    deviceState->dm_x1 = deviceState->m_dot_h_dt * deviceState->m_x1 -
        deviceState->h_eff_dt_x1;
    deviceState->dm_x2 = deviceState->m_dot_h_dt * deviceState->m_x2 -
        deviceState->h_eff_dt_x2;

    /* Debugging */
    // Fixes it
    //deviceState->exfil_chan_0 = m_dot_h_dt;
    //deviceState->exfil_chan_1 = m_dot_h_dt * deviceState->m_x2;
    //deviceState->exfil_chan_2 = h_eff_dt_x2;

    // Fixes it
    //deviceState->exfil_chan_0 = m_dot_h_dt * deviceState->m_x2;

    // Fixes it
    deviceState->tmp = m_dot_h_dt * deviceState->m_x2;

    // Breaks it
    //;

    // Breaks it
    //deviceState->exfil_chan_0 = deviceState->m_x2;

    // Breaks it
    //deviceState->exfil_chan_0 = m_dot_h_dt;

    // Breaks it
    //float exfil_chan_0 = m_dot_h_dt * deviceState->m_x2;

    /* Step forward in time (Euler). Note that dt is incorporated into the
     * effective field coefficients (so make the FPU less confused). */
    deviceState->iteration++;
    deviceState->m_x0 += deviceState->dm_x0;
    deviceState->m_x1 += deviceState->dm_x1;
    deviceState->m_x2 += deviceState->dm_x2;

    /* We did it! This variable notifies the ReadyToSend (RTS) handler that
     * we've just updated. Once read, the output pin handler will set this to
     * zero, so that successive RTSs don't send a message unless a new state
     * can be computed. */
    deviceState->did_we_just_update = 1;
}
