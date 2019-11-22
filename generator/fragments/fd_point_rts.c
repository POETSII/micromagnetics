if (deviceState->did_we_just_update || deviceState->is_initialised == 0)
{
    *readyToSend |= RTS_FLAG_state_push;

    /* Send to the supervisor every eight time steps. */
    //if (((deviceState->iteration >> 3) << 3) == deviceState->iteration)
    /* Send to the supervisor if I'm x0=0. */
    if (deviceProperties->x0 == 0)
        *readyToSend |= RTS_FLAG_exfiltrate;

    /* Send to the supervisor only if we're device zero.
    if (deviceProperties->x0 == 0) *readyToSend |= RTS_FLAG_exfiltrate;
    */
}
