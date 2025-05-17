#ifndef __PID_INST_H__
#define __PID_INST_H__

#include <controller/axis.h>

enum pid_inst {
    PID_ANGLE,
    PID_RATE,
    PID_INST_NUM,
};

float pid_inst_process(enum pid_inst pid, enum axis axis, float in);
void pid_inst_tune(enum pid_inst pid, enum axis axis, float Kd, float Ki, float Kp);
void pid_inst_reset(enum pid_inst pid, enum axis axis);
void pid_inst_reset_all(void);

#endif  // __PID_INST_H__
