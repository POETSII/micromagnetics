    /* Normalise using Fast Inverse Square Root. Never thought I'd need this
     * again! Two Newton-Raphson iterations gets us to e-6 accuracy at
     * |m|=1. */
    target = (DEVICESTATE(m_x0) * DEVICESTATE(m_x0)) +
        (DEVICESTATE(m_x1) * DEVICESTATE(m_x1)) +
        (DEVICESTATE(m_x2) * DEVICESTATE(m_x2));
    fisr = {target};
    fisr.i = 0x5f3759df - ( fisr.i >> 1 );
    fisr.f *= (1.5 - ( target * 0.5 * fisr.f * fisr.f));  /* First NR */
    fisr.f *= (1.5 - ( target * 0.5 * fisr.f * fisr.f));  /* Second NR */
    DEVICESTATE(m_x0) *= fisr.f;
    DEVICESTATE(m_x1) *= fisr.f;
    DEVICESTATE(m_x2) *= fisr.f;