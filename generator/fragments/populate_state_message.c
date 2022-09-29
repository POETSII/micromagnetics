/* Populate outbound state message. */
int dim;
for (dim = 0; dim < {{v:dim}}; dim++) MSG(x)[dim] = DEVICEPROPERTIES(x)[dim];
MSG(iteration) = DEVICESTATE(iteration);
MSG(m_x)[0] = DEVICESTATE(m_x)[0];
MSG(m_x)[1] = DEVICESTATE(m_x)[1];
MSG(m_x)[2] = DEVICESTATE(m_x)[2];

/* Set this to prevent on ReadyToSend from trying to fire another message
 * immediately after this one. */
DEVICESTATE(we_can_update_now) = 0;

/* Set this to ensure we update our state for future iterations. */
DEVICESTATE(is_initialised) = 1;
