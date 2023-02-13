#include <math.h> /** TODO: Consider using cmsis fastmath */
#include "attitude.h"
#include "quaternion.h"

/** See: https://folk.ntnu.no/skoge/prost/proceedings/ecc-2013/data/papers/0927.pdf
  * Access 24.04.2023
  *
  * For more useful info about converting quaternion to euler see:
  * http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
  *
  * TODO: Handle singularities.
  * TODO: cmsis fastmath could probably be used insted of math.h
  */
void attitude_quaternion_to_euler(float q[4], float ang[3]) {
    float tmp_1, tmp_2;
    float sqw = q[0] * q[0];
    float sqx = q[1] * q[1];
    float sqy = q[2] * q[2];
    float sqz = q[3] * q[3];
    // roll
    tmp_1 = 2.0f * (q[0] * q[1] + q[2] * q[3]);
    tmp_2 = sqw - sqx - sqy + sqz;
    ang[1] = atan2f(tmp_1, tmp_2);
    // pitch
    tmp_1 = 2.0f * (q[0] * q[2] - q[3] * q[1]);
    ang[0] = asin(tmp_1);
    // yaw
    tmp_1 = 2.0f * (q[0] * q[3] + q[1] * q[2]);
    tmp_2 = sqw + sqx - sqy - sqz;
    ang[2] = atan2f(tmp_1, tmp_2);
};

static void integrate_quaternion(float q_att[4], float q_dt[4], int dt) {
    for (int i = 0; i < 4; i++) {
        q_att[i] += q_dt[i] * (dt / 1000.0f);
    }
}

void attitude_get_quaternion(float q_att[4], float ang_v[3], int dt) {
    float q_dt[4] = {1.0f, 0.0f, 0.0f, 0.0f};
    float q_v[4] = {0.0f, ang_v[0], ang_v[1], ang_v[2]};

    quaternion_product(q_att, q_v, q_dt);
    for (int i = 0; i < 4; i++) {
        q_dt[i] *= 0.5f;
    }
    /**
     * TODO: Don't run operations in-place?
     */
    quaternion_normalize(q_dt, q_dt);
    integrate_quaternion(q_att, q_dt, dt);
    quaternion_normalize(q_att, q_att);
}
