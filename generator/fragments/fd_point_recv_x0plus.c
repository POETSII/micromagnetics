DEVICESTATE(iteration_x0plus) = MSG(iteration);
if (DEVICESTATE(iteration_x0plus) & 1)
{
    DEVICESTATE(m_x0_x0plus_odd) = MSG(m_x0);
    DEVICESTATE(m_x1_x0plus_odd) = MSG(m_x1);
    DEVICESTATE(m_x2_x0plus_odd) = MSG(m_x2);
}
else
{
    DEVICESTATE(m_x0_x0plus_even) = MSG(m_x0);
    DEVICESTATE(m_x1_x0plus_even) = MSG(m_x1);
    DEVICESTATE(m_x2_x0plus_even) = MSG(m_x2);
}