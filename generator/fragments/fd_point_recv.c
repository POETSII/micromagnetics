for (int dim = 0; dim < {{v:dim}}; dim++)
{
    if (MSG(x)[dim] < DEVICEPROPERTIES(x)[dim])
    {
        DEVICESTATE(iteration_neighbour[dim][0]) = MSG(iteration);
        if (DEVICESTATE(iteration_neighbour[dim][0]) & 1)
        {
            DEVICESTATE(m_x_neighbour)[1][dim][0][0] = MSG(m_x)[0];
            DEVICESTATE(m_x_neighbour)[1][dim][0][1] = MSG(m_x)[1];
            DEVICESTATE(m_x_neighbour)[1][dim][0][2] = MSG(m_x)[2];
        }
        else
        {
            DEVICESTATE(m_x_neighbour)[0][dim][0][0] = MSG(m_x)[0];
            DEVICESTATE(m_x_neighbour)[0][dim][0][1] = MSG(m_x)[1];
            DEVICESTATE(m_x_neighbour)[0][dim][0][2] = MSG(m_x)[2];
        }
    }
    else
    {
        if (DEVICESTATE(iteration_neighbour[dim][1]) & 1)
        {
            DEVICESTATE(m_x_neighbour)[1][dim][1][0] = MSG(m_x)[0];
            DEVICESTATE(m_x_neighbour)[1][dim][1][1] = MSG(m_x)[1];
            DEVICESTATE(m_x_neighbour)[1][dim][1][2] = MSG(m_x)[2];
        }
        else
        {
            DEVICESTATE(m_x_neighbour)[0][dim][1][0] = MSG(m_x)[0];
            DEVICESTATE(m_x_neighbour)[0][dim][1][1] = MSG(m_x)[1];
            DEVICESTATE(m_x_neighbour)[0][dim][1][2] = MSG(m_x)[2];
        }
    }
}