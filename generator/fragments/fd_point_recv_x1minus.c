deviceState->iteration_x1minus = message->iteration;
if (deviceState->iteration_x1minus & 1)
{
    deviceState->m_x0_x1minus_odd = message->m_x0;
    deviceState->m_x1_x1minus_odd = message->m_x1;
    deviceState->m_x2_x1minus_odd = message->m_x2;
}
else
{
    deviceState->m_x0_x1minus_even = message->m_x0;
    deviceState->m_x1_x1minus_even = message->m_x1;
    deviceState->m_x2_x1minus_even = message->m_x2;
}
