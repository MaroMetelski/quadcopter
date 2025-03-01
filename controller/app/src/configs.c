#include "zephyr/toolchain/gcc.h"
#include <controller/low_level/configs_storage.h>
#include <controller/configs.h>

#define CONFIGS_GET(configs, key, dest) \
{ \
    if (!dest) { \
        return false; \
    } \
    *dest = configs[key]; \
    return true; \
}

#define CONFIGS_SET(configs, key, src) \
{ \
    if (!src) { \
        return false; \
    } \
    configs[key] = *src; \
    return true; \
}

static struct pid_config pid_configs_rate[CONFIG_PID_LAST];
static struct pid_config pid_configs_angle[CONFIG_PID_LAST];
static struct motor_config motor_configs[CONFIG_MOTOR_LAST];
static struct input_channel_pwm_calib input_calibs[CONFIG_INPUT_CHANNEL_CONFIG_LAST];
static struct input_channel_config input_configs[CONFIG_INPUT_CHANNEL_CONFIG_LAST];
static struct controller_config ctrl_config[1];

bool configs_get_pid_rate(enum pid_config_key key, struct pid_config *dst)
{
    CONFIGS_GET(pid_configs_rate, key, dst);
}

bool configs_set_pid_rate(enum pid_config_key key, struct pid_config *src)
{
    CONFIGS_SET(pid_configs_rate, key, src);
}

bool configs_get_pid_angle(enum pid_config_key key, struct pid_config *src)
{
    CONFIGS_GET(pid_configs_angle, key, src);
}

bool configs_set_pid_angle(enum pid_config_key key, struct pid_config *src)
{
    CONFIGS_SET(pid_configs_angle, key, src);
}

bool configs_get_motor(enum motor_config_key key, struct motor_config *dst)
{
    CONFIGS_GET(motor_configs, key, dst);
}

bool configs_set_motor(enum motor_config_key key, struct motor_config *src)
{
    CONFIGS_SET(motor_configs, key, src);
}

bool configs_get_input_config(
    enum input_config_key key, struct input_channel_config *dst)
{
    CONFIGS_GET(input_configs, key, dst);
}

bool configs_set_input_config(
    enum input_config_key key, struct input_channel_config *src)
{
    CONFIGS_SET(input_configs, key, src);
}

bool configs_get_input_calib(
    enum input_config_key key, struct input_channel_pwm_calib *dst)
{
    CONFIGS_GET(input_calibs, key, dst);
}

bool configs_set_input_calib(
    enum input_config_key key, struct input_channel_pwm_calib *src)
{
    CONFIGS_SET(input_calibs, key, src);
}

bool configs_set_controller(struct controller_config *src)
{
    CONFIGS_SET(ctrl_config, 0, src);
}

bool configs_get_controller(struct controller_config *dst)
{
    CONFIGS_GET(ctrl_config, 0, dst);
}

bool configs_save_all(void)
{
    bool rc = true;
    rc &= configs_storage_save_one("motors", motor_configs, sizeof(motor_configs));
    rc &= configs_storage_save_one("pid/rate", pid_configs_rate, sizeof(pid_configs_rate));
    rc &= configs_storage_save_one("pid/angle", pid_configs_angle, sizeof(pid_configs_angle));
    rc &= configs_storage_save_one("input/config", input_configs, sizeof(input_configs));
    rc &= configs_storage_save_one("input/calib", input_calibs, sizeof(input_calibs));
    rc &= configs_storage_save_one("controller", ctrl_config, sizeof(ctrl_config));
    return rc;
}

bool configs_load_all(void)
{
    bool rc = true;
    rc &= configs_storage_load_one("motors", motor_configs, sizeof(motor_configs));
    rc &= configs_storage_load_one("pid/rate", pid_configs_rate, sizeof(pid_configs_rate));
    rc &= configs_storage_load_one("pid/angle", pid_configs_angle, sizeof(pid_configs_angle));
    rc &= configs_storage_load_one("input/config", input_configs, sizeof(input_configs));
    rc &= configs_storage_load_one("input/calib", input_calibs, sizeof(input_calibs));
    rc &= configs_storage_load_one("controller", ctrl_config, sizeof(ctrl_config));
    return rc;
}
