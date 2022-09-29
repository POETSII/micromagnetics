/* Initialise our state arrays. Not nice. Not pleasant. Optimised by the
 * compiler. */
for (int odd_or_even = 0; odd_or_even < 2; odd_or_even++)
for (int x_dir = 0; x_dir < {{v:dim}}; x_dir++)
for (int ahead_or_behind = 0; ahead_or_behind < 2; ahead_or_behind++)
for (int m_dir = 0; m_dir < 3; m_dir++)
{
    DEVICESTATE(iteration_neighbour)[x_dir][ahead_or_behind] = 0;
    DEVICESTATE(m_x_neighbour)[odd_or_even][x_dir][ahead_or_behind][m_dir] = 0;
}

/* Fires a state push message to our neighbours exactly once, at the beginning
 * of the simulation.
 *
 * A quick guide to return codes:
 *
 * 0) "Don't talk to me or my son ever again", at least until the
 *    next softswitch_onIdle call.
 *
 * non-zero) So that the ReadyToSend handler is called after this. */
return !DEVICESTATE(is_initialised);