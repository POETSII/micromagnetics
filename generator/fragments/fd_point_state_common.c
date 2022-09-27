/* "Our" physical state. All spins are pointing "straight up", unless
 * otherwise stated. */
float m_x[3] = {0, 0, 1};

/* Note that "iteration" starts at one; this is to stop us stepping forward
 * before receiving our neighbours' initial states (which begin at zero). */
uint32_t iteration = 1;

/* Have we sent our initial packet to anyone? */
uint8_t is_initialised = 0;

/* Housekeeping */
uint8_t we_can_update_now = 0;
uint8_t done = 0;