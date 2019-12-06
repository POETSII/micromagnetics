if (deviceState->did_we_just_update || deviceState->is_initialised == 0)
{
    *readyToSend |= RTS_FLAG_state_push;

    /* Exfiltration modes, either:
     *  - send to the supervisor every eight time steps.
     *  - send to the supervisor only if we're device zero.
     *  - send to the supervisor only if we've hit the final state.
     *
     * Using one of these predicates:
     *
     *  - if (((deviceState->iteration >> 3) << 3) == deviceState->iteration)
     *  - if (deviceProperties->x0 == 0)
     *  - if (deviceState->iteration >= deviceProperties->finishLine) */

    if (deviceState->iteration >= deviceProperties->finishLine)
        *readyToSend |= RTS_FLAG_exfiltrate
}
