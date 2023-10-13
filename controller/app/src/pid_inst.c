#include <controller/pid.h>
#include <controller/pid_inst.h>
#include <controller/axis.h>

struct pid pid_inst[AXIS_NUM];

void pid_inst_tune(enum axis axis, float Kd, float Ki, float Kp) {
    pid_inst[axis].Kd = Kd;
    pid_inst[axis].Kp = Kp;
    pid_inst[axis].Ki = Ki;
    pid_init(&pid_inst[axis], true);
}

float pid_inst_process(enum axis axis, float in) {
    return pid_process(&pid_inst[axis], in);
}
