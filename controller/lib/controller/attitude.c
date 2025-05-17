#include <math.h>
#include <controller/attitude.h>
#include <controller/calc.h>

void attitude_euler_from_acc(float angles[2], float acc[3]) {
    angles[0] = atan2(-1 * acc[0], sqrt(acc[1] * acc[1] + acc[2] * acc[2]));
    angles[1] = atan2(acc[1], acc[2]);
}
