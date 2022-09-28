/* Our "physical" state. All spins are pointing "straight up", unless otherwise
 * stated. */
float m_x[3] = {0, 0, 1};

/* Note that "iteration" starts at one; this is to stop us stepping forward
 * before receiving our neighbours' initial states (which begin at zero). */
uint32_t iteration = 1;

/* Have we sent our initial packet to anyone? */
uint8_t is_initialised = 0;

/* The state of our neighbours:
 *  - First index: In what direction does the neighbour exist? (always 0 for
 *    the 1d case).
 *  - Second index: Is the neighbour ahead (=1) or behind (=0) in that
 *    direction?
 * Initialised with zeroes in OnInit. */
uint32_t iteration_neighbour[{{v:dim}}][2];

/* And you thought the first one was bad.
 *
 * This monster array is broken up as follows:
 *  - First index: Is the data stored for the odd (=1) or even (=0)
 *    iteration? (think GALS synchronisation).
 *  - Second index: In what direction does the neighbour exist? (always 0 for
 *    the 1d case).
 *  - Third index: Is the neighbour ahead (=1) or behind (=0) in that
 *    direction?
 *  - Fourth index: Index of the magnetic moment vector (always 0, 1, or 2, as
 *    they are three-dimensional vectors).
 * Initialised with zeroes in OnInit. Good luck! */
float m_x_neighbour[2][{{v:dim}}][2][3];

/* Housekeeping */
uint8_t we_can_update_now = 0;
uint8_t done = 0;