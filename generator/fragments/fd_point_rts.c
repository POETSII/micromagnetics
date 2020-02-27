if (deviceState->did_we_just_update || deviceState->is_initialised == 0)
{
    *readyToSend |= RTS_FLAG_state_push;

    /* Starts the timer (device "0" only). */
    if (deviceState->iteration == 1 &&
        deviceProperties->x0 == 0 && deviceProperties->x1 == 0)
        *readyToSend |= RTS_FLAG_starttimer;

    /* Exfiltration modes, either:
     *  - send to the supervisor the first iteration.
     *  - send to the supervisor every eight time steps.
     *  - send to the supervisor only if we're device zero.
     *  - send to the supervisor only if we've hit the final state.
     *
     * Using one of these predicates:
     *
     *  - if (deviceState->iteration == 1)
     *  - if (((deviceState->iteration >> 3) << 3) == deviceState->iteration)
     *  - if (deviceProperties->x0 == 0 && deviceProperties->x1 == 0)
     *  - if (deviceState->iteration >= deviceProperties->finishLine) */

    /* Exfiltrates (iteration == some value) */
    if (deviceState->iteration == deviceProperties->finishLine)
    {
        *readyToSend |= RTS_FLAG_exfiltrate;
    }
}
