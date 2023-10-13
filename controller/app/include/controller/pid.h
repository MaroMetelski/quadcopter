#ifndef __PID_H__
#define __PID_H__

#include <stdbool.h>

#define PID_STATE_LEN 3

struct pid {
    float A0;
    float A1;
    float A2;
    float state[PID_STATE_LEN];
    float Kp;
    float Ki;
    float Kd;
};

void pid_init(struct pid *pid, bool reset_state);
void pid_reset(struct pid *pid);
float pid_process(struct pid *pid, float in);

#endif  // __PID_H__
