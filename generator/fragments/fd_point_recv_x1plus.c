deviceState->iteration_x1plus = message->iteration;
if (deviceState->iteration_x1plus & 1)
{
    deviceState->m_x0_x1plus_odd = message->m_x0;
    deviceState->m_x1_x1plus_odd = message->m_x1;
    deviceState->m_x2_x1plus_odd = message->m_x2;
}
else
{
    deviceState->m_x0_x1plus_even = message->m_x0;
    deviceState->m_x1_x1plus_even = message->m_x1;
    deviceState->m_x2_x1plus_even = message->m_x2;
}