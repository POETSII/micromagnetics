/* Effective field, multiplied by time differential in the x0, x1, and x2
 * directions. */
float h_eff_dt_x[3];

/* The x0 (or x1 or x2) -component of the magnetic moment field value at the
 * neighbour one ahead in the x0 direction in the domain. */
float m_x0_x0plus;
float m_x1_x0plus;
float m_x2_x0plus;

/* As above, but for one behind in the x0 direction. */
float m_x0_x0minus;
float m_x1_x0minus;
float m_x2_x0minus;

/* The magnetic moment field differential for this iteration, derived from
 * Euler updating, in the x0, x1, and x2 directions in the codomain. */
float dm_x[3];

float m_dot_h_dt;
float target;
union {
    float f;
    uint32_t i;
} fisr;    /* For fast-inverse square root. */