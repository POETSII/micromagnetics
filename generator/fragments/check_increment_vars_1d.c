uint8_t goingToUpdate;  /* 1 if our neighbours' iteration is >= ours. */

float h_eff_dt_x0;  /* Effective field, multiplied by time differential... */
float h_eff_dt_x1;  /* ...in the x0, x1, and x2 directions. Signage is... */
float h_eff_dt_x2;  /* ...questionable. */

float m_x0_x0plus;  /* The x0 (or x1 or x2) -component of the magnetic... */
float m_x1_x0plus;  /* ...moment field value at the neighbour one ahead... */
float m_x2_x0plus;  /* ...in the x0 direction in the domain. */

float m_x0_x0minus;  /* As above, but for one behind in the x0 direction. */
float m_x1_x0minus;
float m_x2_x0minus;

float dm_x0;  /* The magnetic moment field differential for this... */
float dm_x1;  /* iteration, derived from Euler updating, in the x0, x1, and */
float dm_x2;  /* x2 directions in the codomain. */

float m_dot_h_dt;
float target;
union {
    float f;
    uint32_t i;
} fisr;    /* For fast-inverse square root. */
