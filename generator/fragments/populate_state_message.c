/* Populate outbound state message. */
int dim;
for (dim = 0; dim < {{v:dim}}; dim++) MSG(x)[dim] = DEVICEPROPERTIES(x)[dim];
MSG(iteration) = DEVICESTATE(iteration);
for (int m_dim = 0; m_dim < 3; m_dim++)
    MSG(m_x)[m_dim] = DEVICESTATE(m_x)[m_dim];

/* Set this to prevent on ReadyToSend from trying to fire another message
 * immediately after this one. */
DEVICESTATE(we_can_update_now) = 0;

/* Set this to ensure we update our state for future iterations. */
DEVICESTATE(is_initialised) = 1;