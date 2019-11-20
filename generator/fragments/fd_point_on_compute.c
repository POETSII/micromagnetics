/* Fires a state push message to our neighbours exactly once, at the
 * "beginning" of the simulation (for iteration zero). */

if (deviceState->is_initialised == 0)
{
    deviceState->is_initialised = 1;
    *readyToSend |= RTS_FLAG_state_push;
    return 1;  /* 1) So that the ReadyToSend handler is called after this. */
}
return 0;  /* 0) "Don't talk to me or my son ever again", at least until the
            * next softswitch_onIdle call. */
