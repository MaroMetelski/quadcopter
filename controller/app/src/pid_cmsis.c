#include <arm_math.h>
#include <app/pid.h>

void pid_init(struct pid *pid, bool reset_state) {
    arm_pid_instance_f32 *arm_pid = (arm_pid_instance_f32 *)pid;
    arm_pid_init_f32(arm_pid, reset_state);
}

void pid_reset(struct pid *pid) {
    arm_pid_instance_f32 *arm_pid = (arm_pid_instance_f32 *)pid;
    arm_pid_reset_f32(arm_pid);
}

float pid_process(struct pid *pid, float in) {
    arm_pid_instance_f32 *arm_pid = (arm_pid_instance_f32 *)pid;
    return arm_pid_f32(arm_pid, in);
}
