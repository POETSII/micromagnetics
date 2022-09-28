/* Where am I? */
#define DIM {{v:dim}}
uint16_t x[DIM];
#undef DIM

/* When are we done? (does not stop the simulation, only governs exfiltration
 * logic for the final interation. */
uint32_t finishLine = {{v:final_iteration}};

/* Effective field coefficients, including the time differential and scaling
 * parameters to ease the burden on the FPU/optimiser. Currently:
 *
 *  - exchange_coeff = 2*A*(dt)*gamma/h/mu_0/Ms*damping
 *  - dmi_coeff = D*(dt)*gamma/2/h/mu_0/Ms*damping
 *  - zeeman_coeff = -H*(dt)*gamma*damping
 *
 * Using:
 *  - A = 4e-13 [J][m-1]
 *  - D = 2.7e-4 [J][m-2]
 *  - Ms = 9.5e4 [A][m-1]
 *  - mu_0 = 4e-7 * pi [kg][s-2][A-2]
 *  - H = 15.4 * Ms (or zero)
 *  - dt = 1e-13 [s]
 *  - h = 1e-9 [m]
 *  - gamma = 2.211e5 [A][s][kg-1]
 *  - alpha = 1
 *  - damping = alpha / (1 + alpha^2)
 *
 * And where the Zeeman field is in the (0, 0, 1) direction when H is
 * positive. */
float exchange_coeff_dt = 7.409e-11;
float dmi_coeff_dt = 1.250e-2;
float zeeman_coeff_dt = -6.301e-3;
