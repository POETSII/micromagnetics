/* Our "physical" state - the magnetic moment vector at this point. Note that
 * it might be more efficient to define this using two angles (i.e. an m_theta
 * representation), as we have a stray degree of freedom here, but I suspect
 * that the computation may be more expensive.
 *
 * All spins are pointing "straight up", unless otherwise stated. */
float m_x[3] = {0, 0, 1};

/* Note that "iteration" starts at one; this is to stop us stepping forward
 * before receiving our neighbours' initial states (which begin at zero). */
uint32_t iteration = 1;

/* Have we sent our initial packet yet? */
uint8_t is_initialised = 0;

/* The iteration value our neighbours are at, for GALS purposes:
 *  - First index: In what direction does the neighbour exist? (always 0 for
 *    the 1d case).
 *  - Second index: Is the neighbour ahead (=1) or behind (=0) in that
 *    direction?
 * Initialised with zeroes in OnInit. */
uint32_t iteration_neighbour[{{v:dim}}][2];

/* And you thought the first one was bad - this monster of an array holds
 * magnetic moment vectors of neighbours at different iterations. It is broken
 * up as follows:
 *
 *  - First index: Is the data stored for the odd (=1) or even (=0)
 *    iteration? (think GALS synchronisation).
 *
 *  - Second index: In what direction does the neighbour exist? (always 0 for
 *    the 1d case).
 *
 *  - Third index: Is the neighbour ahead (=1) or behind (=0) in that
 *    direction?
 *
 *  - Fourth index: Index of the magnetic moment vector (always 0, 1, or 2, as
 *    they are three-dimensional vectors).
 *
 * Initialised with zeroes in OnInit. Good luck! */
float m_x_neighbour[2][{{v:dim}}][2][3];

/* Housekeeping */
uint8_t we_can_update_now = 0;
uint8_t done = 0;