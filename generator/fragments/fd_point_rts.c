if (deviceState->did_we_just_update || deviceState->is_initialised == 0)
{
    *readyToSend |= RTS_FLAG_state_push;

    /* Send to the supervisor every ten time steps. */
    if (deviceState->iteration & 10) *readyToSend |= RTS_FLAG_exfiltrate;
}
