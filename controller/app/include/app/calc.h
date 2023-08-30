#ifndef __CALC_H__
#define __CALC_H__

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

void calc_integrate(float *value, float *d, float n_terms, float dt);

#endif  // __CALC_H__
