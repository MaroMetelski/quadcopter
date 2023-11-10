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

static struct pid_config pid_configs[CONFIG_PID_LAST];
static struct motor_config motor_configs[CONFIG_MOTOR_LAST];
static struct input_channel_pwm_calib input_calibs[CONFIG_INPUT_CHANNEL_CONFIG_LAST];
static struct input_channel_config input_configs[CONFIG_INPUT_CHANNEL_CONFIG_LAST];

bool configs_get_pid(enum pid_config_key key, struct pid_config *dst)
{
    CONFIGS_GET(pid_configs, key, dst);
}

bool configs_set_pid(enum pid_config_key key, struct pid_config *src)
{
    CONFIGS_SET(pid_configs, key, src);
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

bool configs_save_all(void)
{
    bool rc = true;
    rc &= configs_storage_save_one("motors", motor_configs, sizeof(motor_configs));
    rc &= configs_storage_save_one("pid", pid_configs, sizeof(pid_configs));
    rc &= configs_storage_save_one("input/config", input_configs, sizeof(input_configs));
    rc &= configs_storage_save_one("input/calib", input_calibs, sizeof(input_calibs));
    return rc;
}

bool configs_load_all(void)
{
    bool rc = true;
    rc &= configs_storage_load_one("motors", motor_configs, sizeof(motor_configs));
    rc &= configs_storage_load_one("pid", pid_configs, sizeof(pid_configs));
    rc &= configs_storage_load_one("input/config", input_configs, sizeof(input_configs));
    rc &= configs_storage_load_one("input/calib", input_calibs, sizeof(input_calibs));
    return rc;
}
