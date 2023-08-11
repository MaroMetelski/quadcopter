#include <app/calc.h>
#include <app/filter.h>

void filter_complimentary(float *input1, float *input2, float *output, size_t len, float alpha) {
    alpha = MIN(1.0, alpha);
    alpha = MAX(0.0, alpha);

    for (int i = 0; i < len; i++) {
        output[i] = alpha * input1[i] +  (1.0 - alpha) * input2[i];
    }
}
