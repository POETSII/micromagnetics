if (DEVICESTATE(we_can_update_now) or DEVICESTATE(is_initialised) == 0)
{
    RTS(state_push);

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

    /* Exfiltrates (iteration >= some value, and we've not exfiltrated
     * before) */
    if (deviceState->iteration >= deviceProperties->finishLine &&
        deviceState->done == 0)
    {
        RTSSUP();
    }
}