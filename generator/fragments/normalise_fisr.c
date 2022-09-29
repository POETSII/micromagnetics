/* Normalise using Fast Inverse Square Root. Never thought I'd need this again!
 * Two Newton-Raphson iterations gets us to e-6 accuracy at |m|=1. */
float target = 0;
for (int m_dim = 0; m_dim < 3; m_dim++)
    target += DEVICESTATE(m_x)[m_dim] * DEVICESTATE(m_x)[m_dim];

union {
    float f;
    uint32_t i;
} fisr = {target};
fisr.i = 0x5f3759df - ( fisr.i >> 1 );
for (int it = 0; it < 2; it++)  /* Two NR iterations */
    fisr.f *= (1.5 - ( target * 0.5 * fisr.f * fisr.f));

for (int m_dim = 0; m_dim < 3; DEVICESTATE(m_x)[m_dim++] *= fisr.f);