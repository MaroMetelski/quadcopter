#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

#include <drivers/pwm_motors.h>

LOG_MODULE_REGISTER(pwm_motors, LOG_LEVEL_DBG);

#define DT_DRV_COMPAT pwm_motors

struct pwm_motors_config {
    int num_motors;
    const struct pwm_dt_spec *motors;
};

int pwm_motors_set(const struct device *dev, uint8_t channel,
    uint32_t period, uint32_t pulse, pwm_flags_t flags)
{
    const struct pwm_motors_config *cfg = dev->config;
    if (channel > cfg->num_motors) {
        return -EINVAL;
    }
    const struct pwm_dt_spec *motor_pwm = &cfg->motors[channel];
    return pwm_set(motor_pwm->dev, motor_pwm->channel, period, pulse, flags);
}

#define PWM_MOTORS_DEVICE(index) \
\
static const struct pwm_dt_spec pwm_motors_##index[] = { \
    DT_INST_FOREACH_CHILD_SEP(index, PWM_DT_SPEC_GET, (,)) \
}; \
\
static const struct pwm_motors_config pwm_motors_cfg_##index = { \
    .num_motors = ARRAY_SIZE(pwm_motors_##index), \
    .motors = pwm_motors_##index, \
}; \
\
DEVICE_DT_INST_DEFINE(index, NULL, NULL, NULL, \
    &pwm_motors_cfg_##index, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEVICE, NULL);

DT_INST_FOREACH_STATUS_OKAY(PWM_MOTORS_DEVICE);
