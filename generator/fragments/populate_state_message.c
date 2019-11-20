message->iteration = deviceState->iteration;
message->m_x0 = deviceState->m_x0;
message->m_x1 = deviceState->m_x1;
message->m_x2 = deviceState->m_x2;

/* Set this to prevent on ReadyToSend from trying to fire another message after
 * this one. */
deviceState->did_we_just_update = 0;
