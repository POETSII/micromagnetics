iteration_x0plus = message->iteration;
if (iteration_x0plus % 2)
{
    m_x0plus_odd = message->m_x0;
    m_x1plus_odd = message->m_x1;
    m_x2plus_odd = message->m_x2;
}
else
{
    m_x0_x0plus_even = message->m_x0;
    m_x1_x0plus_even = message->m_x1;
    m_x2_x0plus_even = message->m_x2;
}
{{check_increment_state_1d.c}}
