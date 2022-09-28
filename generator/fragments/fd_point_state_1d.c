{{f:fd_point_state_common.c}}

/* The state of our neighbours:
 *  - First index: In what direction does the neighbour exist? (always 0 for
 *    the 1d case).
 *  - Second index: Is the neighbour ahead (=1) or behind (=0) in that
 *    direction?
 */
uint32_t iteration_neighbour[1][2] = {{0, 0}};

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
 * Good luck! */
float m_x_neighbour[2][1][2][3] = {
    { /* Even */
        {  /* Neighbours in direction zero */
            {0, 0, 0},  /* Ahead */
            {0, 0, 0}   /* Behind */
        }
    },
    { /* Odd */
        { /* Neighbours in direction zero */
            {0, 0, 0}, /* Ahead */
            {0, 0, 0}  /* Behind */
        }
    }
};