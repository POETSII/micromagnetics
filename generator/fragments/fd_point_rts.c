if (deviceState->did_we_just_update || deviceState->is_initialised == 0)
{
    *readyToSend |= RTS_FLAG_state_push;

    /* Send to the supervisor every eight time steps. */
    if (((deviceState->iteration >> 3) << 3) == deviceState->iteration)
        *readyToSend |= RTS_FLAG_exfiltrate;
}
