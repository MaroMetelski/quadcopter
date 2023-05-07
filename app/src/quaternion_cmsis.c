#include <arm_math.h>
#include <app/quaternion.h>

void quaternion_product(float *qa, float *qb, float *out) {
    return arm_quaternion_product_single_f32(qa, qb, out);
}

void quaternion_normalize(float *qa, float *out) {
    return arm_quaternion_normalize_f32(qa, out, 1);
}
