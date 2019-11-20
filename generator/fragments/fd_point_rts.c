if (did_we_just_update)
{
    did_we_just_update = 0;
    *readyToSend |= RTS_FLAG_state_push;

    /* Send to the supervisor every ten time steps. */
    if (iteration % 10 == 0) *readyToSend |= RTS_FLAG_exfiltrate;
}
