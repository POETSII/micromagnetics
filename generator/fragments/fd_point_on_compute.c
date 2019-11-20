/* Fires a state push message to our neighbours exactly once, at the
 * "beginning" of the simulation (for iteration zero). */

if (is_initialised == 0)
{
    is_initialised = 1;
    *readyToSend |= RTS_FLAG_state_push;
    return 1;
}
return 0;  /* i.e. "Don't talk to me or my son ever again" */
