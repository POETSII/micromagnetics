MSG(iteration) = DEVICESTATE(iteration);
MSG(m_x0) = DEVICESTATE(m_x0);
MSG(m_x1) = DEVICESTATE(m_x1);
MSG(m_x2) = DEVICESTATE(m_x2);

/* Set this to prevent on ReadyToSend from trying to fire another message after
 * this one. */
DEVICESTATE(did_we_just_update) = 0;

/* Set this to prevent OnIdle from trying to fire another initialisation
 * message. */
DEVICESTATE(is_initialised) = 1;