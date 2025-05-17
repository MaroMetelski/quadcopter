#include <controller/config_dispatcher.h>
#include <controller/configs.h>
#include <controller/pid_inst.h>
#include <controller/motors.h>
#include <controller/input.h>

#ifdef CONFIG_APP_MOTORS

static enum motor config_key_to_motor(enum motor_config_key key)
{
    switch (key) {
    case CONFIG_MOTOR_BL_CONFIG:
        return MOTOR_BACK_LEFT;
    case CONFIG_MOTOR_FR_CONFIG:
        return MOTOR_FRONT_RIGHT;
    case CONFIG_MOTOR_FL_CONFIG:
        return MOTOR_FRONT_LEFT;
    case CONFIG_MOTOR_BR_CONFIG:
        return MOTOR_BACK_RIGHT;
    default:
        return -1;
    }
    return -1;
}

bool config_dispatcher_update_motors(void)
{
    for (int i = 0; i < CONFIG_MOTOR_LAST; i++) {
        struct motor_config cfg;

        if (!configs_get_motor(i, &cfg)) {
            return false;
        }
        if (!motor_configure(config_key_to_motor(i), &cfg)) {
            return false;
        }
    }
    return true;
}

#else

bool config_dispatcher_update_motors(void)
{
    return true;
}

#endif

static enum axis config_key_to_pid_axis(enum pid_config_key key)
{
    switch (key) {
    case CONFIG_PID_PITCH:
        return AXIS_PITCH;
    case CONFIG_PID_ROLL:
        return AXIS_ROLL;
    case CONFIG_PID_YAW:
        return AXIS_YAW;
    default:
        return -1;
    }
    return -1;
}

bool config_dispatcher_update_pid_rate(void)
{
    for (int i = 0; i < CONFIG_PID_LAST; i++) {
        struct pid_config cfg;

        if (!configs_get_pid_rate(i, &cfg)) {
            return false;
        }
        pid_inst_tune(PID_RATE, config_key_to_pid_axis(i), cfg.Kd, cfg.Ki, cfg.Kp);
    }
    return true;
}

bool config_dispatcher_update_pid_angle(void)
{
    for (int i = 0; i < CONFIG_PID_LAST; i++)
    {
        struct pid_config cfg;

        if (!configs_get_pid_angle(i, &cfg)) {
            return false;
        }
        pid_inst_tune(PID_ANGLE, config_key_to_pid_axis(i), cfg.Kd, cfg.Ki, cfg.Kp);
    }
    return true;
}

bool config_dispatcher_update_controller_mode(void)
{
    // TODO: Add controller module and API to choose rate / angle mode.
    return true;
}

#ifdef CONFIG_APP_INPUT

static enum input_channel config_key_to_input_channel(enum input_config_key key)
{
    switch (key) {
    case CONFIG_INPUT_CHANNEL_PITCH_CONFIG:
        return CHANNEL_PITCH;
    case CONFIG_INPUT_CHANNEL_ROLL_CONFIG:
        return CHANNEL_ROLL;
    case CONFIG_INPUT_CHANNEL_YAW_CONFIG:
        return CHANNEL_YAW;
    case CONFIG_INPUT_CHANNEL_THROTTLE_CONFIG:
        return CHANNEL_THROTTLE;
    default:
        return -1;
    }
    return -1;
}

bool config_dispatcher_update_input(void)
{
    for (int i = 0; i < CONFIG_INPUT_CHANNEL_CONFIG_LAST; i++) {
        struct input_channel_config cfg;
        struct input_channel_pwm_calib calib;
        if (!configs_get_input_config(i, &cfg)) {
            return false;
        }
        if (!configs_get_input_calib(i, &calib)) {
            return false;
        }
        if (!input_configure_channel(config_key_to_input_channel(i), &cfg)) {
            return false;
        }
        if (!input_set_calibration(config_key_to_input_channel(i), &calib)) {
            return false;
        }
    }
    return true;
}

#else

bool config_dispatcher_update_input(void)
{
    return true;
}

#endif

bool config_dispatcher_update_all(void)
{
    return (config_dispatcher_update_motors()
        && config_dispatcher_update_input()
        && config_dispatcher_update_pid_rate()
        && config_dispatcher_update_pid_angle());
}
