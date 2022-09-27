/* Now we need to check if our neighbours satisfy the update condition. If they
 * do, we update our state. */
if (DEVICESTATE(iteration_x0plus) >= DEVICESTATE(iteration) and
    DEVICESTATE(iteration_x0minus) >= DEVICESTATE(iteration))
{
    /* This variable notifies the ReadyToSend (RTS) handler that
     * we've just updated. Once read, the output pin handler will set this to
     * zero, so that successive RTSs don't send a message unless a new state
     * can be computed. */
    DEVICESTATE(we_can_update_now) = 1;
}