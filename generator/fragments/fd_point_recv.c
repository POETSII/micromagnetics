if (MSG(x)[0] < DEVICEPROPERTIES(x)[0])
{
    DEVICESTATE(iteration_x0minus) = MSG(iteration);
    if (DEVICESTATE(iteration_x0minus) & 1)
    {
        DEVICESTATE(m_x0_x0minus_odd) = MSG(m_x)[0];
        DEVICESTATE(m_x1_x0minus_odd) = MSG(m_x)[1];
        DEVICESTATE(m_x2_x0minus_odd) = MSG(m_x)[2];
    }
    else
    {
        DEVICESTATE(m_x0_x0minus_even) = MSG(m_x)[0];
        DEVICESTATE(m_x1_x0minus_even) = MSG(m_x)[1];
        DEVICESTATE(m_x2_x0minus_even) = MSG(m_x)[2];
    }
}
else
{
    if (DEVICESTATE(iteration_x0plus) & 1)
    {
        DEVICESTATE(m_x0_x0plus_odd) = MSG(m_x)[0];
        DEVICESTATE(m_x1_x0plus_odd) = MSG(m_x)[1];
        DEVICESTATE(m_x2_x0plus_odd) = MSG(m_x)[2];
    }
    else
    {
        DEVICESTATE(m_x0_x0plus_even) = MSG(m_x)[0];
        DEVICESTATE(m_x1_x0plus_even) = MSG(m_x)[1];
        DEVICESTATE(m_x2_x0plus_even) = MSG(m_x)[2];
    }
}