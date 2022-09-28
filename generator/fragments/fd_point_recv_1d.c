if (MSG(x)[0] < DEVICEPROPERTIES(x)[0])
{
    DEVICESTATE(iteration_x0minus) = MSG(iteration);
    if (DEVICESTATE(iteration_x0minus) & 1)
    {
        DEVICESTATE(m_x_neighbour)[1][0][0][0] = MSG(m_x)[0];
        DEVICESTATE(m_x_neighbour)[1][0][0][1] = MSG(m_x)[1];
        DEVICESTATE(m_x_neighbour)[1][0][0][2] = MSG(m_x)[2];
    }
    else
    {
        DEVICESTATE(m_x_neighbour)[0][0][0][0] = MSG(m_x)[0];
        DEVICESTATE(m_x_neighbour)[0][0][0][1] = MSG(m_x)[1];
        DEVICESTATE(m_x_neighbour)[0][0][0][2] = MSG(m_x)[2];
    }
}
else
{
    if (DEVICESTATE(iteration_x0plus) & 1)
    {
        DEVICESTATE(m_x_neighbour)[1][0][1][0] = MSG(m_x)[0];
        DEVICESTATE(m_x_neighbour)[1][0][1][1] = MSG(m_x)[1];
        DEVICESTATE(m_x_neighbour)[1][0][1][2] = MSG(m_x)[2];
    }
    else
    {
        DEVICESTATE(m_x_neighbour)[0][0][1][0] = MSG(m_x)[0];
        DEVICESTATE(m_x_neighbour)[0][0][1][1] = MSG(m_x)[1];
        DEVICESTATE(m_x_neighbour)[0][0][1][2] = MSG(m_x)[2];
    }
}