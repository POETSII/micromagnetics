uint8_t did_we_just_update = 0;
uint8_t done = 0;

/* "Our" state. Note that "iteration" starts at one; this is to stop us
 * stepping forward before receiving our neighbours' initial states (which
 * begin at zero).
 *
 * Also, all spins are pointing straight up, unless otherwise stated. */
uint32_t iteration = 1;
float m_x0 = 0;
float m_x1 = 0;
float m_x2 = 1;

/* Have we sent our initial packet to anyone? */
uint8_t is_initialised = 0;
