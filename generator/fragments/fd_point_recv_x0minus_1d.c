iteration_x0minus = message->iteration;
if (iteration_x0minus % 2)
{
    m_x0_x0minus_odd = message->m_x0;
    m_x1_x0minus_odd = message->m_x1;
    m_x2_x0minus_odd = message->m_x2;
}
else
{
    m_x0_x0minus_even = message->m_x0;
    m_x1_x0minus_even = message->m_x1;
    m_x2_x0minus_even = message->m_x2;
}
{{check_increment_state_1d.c}}
