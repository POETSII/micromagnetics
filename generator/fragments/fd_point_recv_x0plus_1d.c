deviceState->iteration_x0plus = message->iteration;
if (deviceState->iteration_x0plus & 1)
{
    deviceState->m_x0_x0plus_odd = message->m_x0;
    deviceState->m_x1_x0plus_odd = message->m_x1;
    deviceState->m_x2_x0plus_odd = message->m_x2;
}
else
{
    deviceState->m_x0_x0plus_even = message->m_x0;
    deviceState->m_x1_x0plus_even = message->m_x1;
    deviceState->m_x2_x0plus_even = message->m_x2;
}
{{check_increment_state_1d.c}}
