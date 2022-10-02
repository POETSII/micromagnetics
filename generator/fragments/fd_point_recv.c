for (int dim = 0; dim < {{v:dim}}; dim++)
{
    /* Behind (and n->0 for periodicity). The first condition asks are you
     * directly behind? and the second condition asks are you really far ahead
     * of me, so far in fact, that I suspect you're a wrap-around device? */
    int ahead_or_behind;
    if (DEVICEPROPERTIES(x)[dim] - MSG(x)[dim] == 1 or
        MSG(x)[dim] > DEVICEPROPERTIES(x)[dim] + 1)
    {
        ahead_or_behind = 0;
    }
    else
    {
        ahead_or_behind = 1;
    }

    /* Update iteration table */
    DEVICESTATE(iteration_neighbour[dim][ahead_or_behind]) = MSG(iteration);

    /* Update magnetisation in the correct buffer. */
    int odd_or_even = MSG(iteration) & 1;
    for (int m_dim = 0; m_dim < 3; m_dim++)
    {
        DEVICESTATE(m_x_neighbour)[odd_or_even][dim][ahead_or_behind][m_dim] = \
            MSG(m_x)[m_dim];
    }
}