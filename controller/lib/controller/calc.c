#include <controller/calc.h>

void calc_integrate(float *value, float *d, float n_terms, float dt) {
    for (int i = 0; i < n_terms; i++) {
        /** Really basic euler method for now. */
        value[i] += d[i] * dt;
    }
}
