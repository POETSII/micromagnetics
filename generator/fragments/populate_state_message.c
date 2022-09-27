MSG(x)[0] = DEVICEPROPERTIES(x)[0];
MSG(iteration) = DEVICESTATE(iteration);
MSG(m_x)[0] = DEVICESTATE(m_x)[0];
MSG(m_x)[1] = DEVICESTATE(m_x)[1];
MSG(m_x)[2] = DEVICESTATE(m_x)[2];

/* Set this to prevent on ReadyToSend from trying to fire another message after
 * this one. */
DEVICESTATE(we_can_update_now) = 0;

/* Set this to prevent OnIdle from trying to fire another initialisation
 * message. */
DEVICESTATE(is_initialised) = 1;