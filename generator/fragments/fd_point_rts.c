/* We send either if our neighbours' iterations have caught up with ours, or if
 * we need to burst our initial state to get things going. */
if (DEVICESTATE(we_can_update_now) or DEVICESTATE(is_initialised) == 0)
{
    RTS(state_send);

    /* Exfiltration modes to the Supervisor. Some ideas:
     *  - send to the supervisor the first iteration.
     *  - send to the supervisor every eight time steps.
     *  - send to the supervisor only if we're device zero.
     *  - send to the supervisor only if we've hit the final state.
     *
     * Using one of these predicates:
     *
     *  - if (DEVICESTATE(iteration) == 1)
     *  - if (((DEVICESTATE(iteration) >> 3) << 3) == DEVICESTATE(iteration))
     *  - if (DEVICEPROPERTIES(x)[0] == 0 && DEVICEPROPERTIES(x)[1] == 0)
     *  - if (DEVICESTATE(iteration) >= GRAPHPROPERTIES(finishLine))
     *
     * The check on state field "done" stops us from sending twice once we're
     * past the finish line. */
    if (((DEVICESTATE(iteration) >> 4) << 4) == DEVICESTATE(iteration) and
        DEVICESTATE(done) == 0)
    {
        RTSSUP();
    }
}