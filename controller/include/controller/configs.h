#ifndef __CONFIGS_H__
#define __CONFIGS_H__

#include <controller/motors.h>
#include <controller/input.h>
#include <controller/controller.h>

enum pid_config_key {
    CONFIG_PID_PITCH,
    CONFIG_PID_ROLL,
    CONFIG_PID_YAW,
    CONFIG_PID_LAST,
};

enum motor_config_key {
    CONFIG_MOTOR_FL_CONFIG,
    CONFIG_MOTOR_FR_CONFIG,
    CONFIG_MOTOR_BL_CONFIG,
    CONFIG_MOTOR_BR_CONFIG,
    CONFIG_MOTOR_LAST,
};

enum input_config_key {
    CONFIG_INPUT_CHANNEL_PITCH_CONFIG,
    CONFIG_INPUT_CHANNEL_YAW_CONFIG,
    CONFIG_INPUT_CHANNEL_ROLL_CONFIG,
    CONFIG_INPUT_CHANNEL_THROTTLE_CONFIG,
    CONFIG_INPUT_CHANNEL_CONFIG_LAST
};

struct pid_config {
    float Kp;
    float Ki;
    float Kd;
};

struct controller_config {
    enum controller_mode mode;
};

bool configs_get_pid_rate(enum pid_config_key key, struct pid_config *dst);
bool configs_set_pid_rate(enum pid_config_key key, struct pid_config *src);

bool configs_get_pid_angle(enum pid_config_key key, struct pid_config *dst);
bool configs_set_pid_angle(enum pid_config_key key, struct pid_config *src);

bool configs_get_motor(enum motor_config_key key, struct motor_config *dst);
bool configs_set_motor(enum motor_config_key key, struct motor_config *src);

bool configs_get_input_config(
    enum input_config_key key, struct input_channel_config *dst);
bool configs_set_input_config(
    enum input_config_key key, struct input_channel_config *src);

bool configs_get_input_calib(
    enum input_config_key key, struct input_channel_pwm_calib *dst);
bool configs_set_input_calib(
    enum input_config_key key, struct input_channel_pwm_calib *src);

bool configs_get_controller(struct controller_config *dst);
bool configs_set_controller(struct controller_config *src);

bool configs_save_all(void);
bool configs_load_all(void);

#endif  // __CONFIGS_H__
