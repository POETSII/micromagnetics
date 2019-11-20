message->iteration = iteration;
message->m_x0 = m_x0;
message->m_x1 = m_x1;
message->m_x2 = m_x2;

/* Set this to prevent on ReadyToSend from trying to fire another message after
 * this one. */
deviceState->did_we_just_update = 0;
