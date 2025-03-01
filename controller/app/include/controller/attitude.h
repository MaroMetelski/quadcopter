#ifndef __SRC_ATTITUDE_H__
#define __SRC_ATTITUDE_H__

void attitude_quaternion_to_euler(float q[4], float ang[3]);
void attitude_euler_from_acc(float angles[2], float acc[3]);
void attitude_propagate_euler(float angles[2], float ang_v[2], float dt);
void attitude_propagate_quaternion(float q_att[4], float ang_v[3], float dt);

#endif  // __SRC_ATTITUDE_H__
