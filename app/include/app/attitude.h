#ifndef __SRC_ATTITUDE_H__
#define __SRC_ATTITUDE_H__

void attitude_quaternion_to_euler(float q[4], float ang[3]);
void attitude_get_quaternion(float q_att[4], float ang_v[3], int dt);
void attitude_get_euler(float ang[3], float ang_v[3], int dt);

#endif  // __SRC_ATTITUDE_H__
