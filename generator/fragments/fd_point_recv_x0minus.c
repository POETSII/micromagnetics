DEVICESTATE(iteration_x0minus) = MSG(iteration);
if (DEVICESTATE(iteration_x0minus) & 1)
{
    DEVICESTATE(m_x0_x0minus_odd) = MSG(m_x0);
    DEVICESTATE(m_x1_x0minus_odd) = MSG(m_x1);
    DEVICESTATE(m_x2_x0minus_odd) = MSG(m_x2);
}
else
{
    DEVICESTATE(m_x0_x0minus_even) = MSG(m_x0);
    DEVICESTATE(m_x1_x0minus_even) = MSG(m_x1);
    DEVICESTATE(m_x2_x0minus_even) = MSG(m_x2);
}