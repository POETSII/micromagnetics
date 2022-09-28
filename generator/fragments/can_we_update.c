/* Now we need to check if our neighbours satisfy the update condition - all
 * neighbours need to be at or ahead of us. */
bool update = true;
for (int x_dir = 0; x_dir < {{v:dim}}; x_dir++)
for (int ahead_or_behind = 0; ahead_or_behind < 2; ahead_or_behind++)
    update = update && \
        DEVICESTATE(iteration_neighbour)[x_dir][ahead_or_behind] >= \
        DEVICESTATE(iteration);

if (update)
{
    /* This variable notifies the ReadyToSend (RTS) handler that
     * we've just updated. Once read, the output pin handler will set this to
     * zero, so that successive RTSs don't send a message unless a new state
     * can be computed. */
    DEVICESTATE(we_can_update_now) = 1;
}