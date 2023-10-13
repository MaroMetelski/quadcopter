#ifndef __PID_INST_H__
#define __PID_INST_H__

#include <controller/axis.h>

float pid_inst_process(enum axis axis, float in);
void pid_inst_tune(enum axis axis, float Kd, float Ki, float Kp);

#endif  // __PID_INST_H__
