/* Fires a state push message to our neighbours exactly once, at the
 * "beginning" of the simulation (for iteration zero).
 *
 * A quick guide to return codes:
 *
 * 0) "Don't talk to me or my son ever again", at least until the
 *    next softswitch_onIdle call.
 * 1) So that the ReadyToSend handler is called after this. */
return !deviceState->is_initialised;
