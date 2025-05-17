#include <controller/pid.h>
#include <controller/pid_inst.h>
#include <controller/axis.h>

struct pid pid_inst[PID_INST_NUM][AXIS_NUM];

void pid_inst_tune(enum pid_inst pid, enum axis axis, float Kd, float Ki, float Kp) {
    pid_inst[pid][axis].Kd = Kd;
    pid_inst[pid][axis].Kp = Kp;
    pid_inst[pid][axis].Ki = Ki;
    pid_init(&pid_inst[pid][axis], true);
}

float pid_inst_process(enum pid_inst pid, enum axis axis, float in) {
    return pid_process(&pid_inst[pid][axis], in);
}

void pid_inst_reset(enum pid_inst pid, enum axis axis) {
    pid_init(&pid_inst[pid][axis], true);
}

void pid_inst_reset_all(void) {
    for (int i = 0; i < PID_INST_NUM; i++) {
        for (int j = 0; j < AXIS_NUM; j++) {
            pid_inst_reset(i, j);
        }
    }
}
