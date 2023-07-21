#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

#include <drivers/pwm_inputs.h>

LOG_MODULE_REGISTER(pwm_inputs, LOG_LEVEL_DBG);

#define DT_DRV_COMPAT pwm_inputs

#define PWM_CHANNEL_READ_TIMEOUT_MS 100

struct pwm_inputs_config {
    int num_inputs;
    const struct pwm_dt_spec *inputs;
};

int pwm_inputs_get_channel_duty_usec(
    const struct device *dev, uint64_t *data, uint8_t channel)
{
    const struct pwm_inputs_config *cfg = dev->config;
    if (channel > cfg->num_inputs) {
        return -EINVAL;
    }
    const struct pwm_dt_spec *pwm_input = &cfg->inputs[channel];
    uint64_t _period;
    return pwm_capture_usec(
        pwm_input->dev, pwm_input->channel,
        PWM_CAPTURE_TYPE_PULSE, &_period, data,
        K_MSEC(PWM_CHANNEL_READ_TIMEOUT_MS)
    );
};

int pwm_inputs_get_num_channels(const struct device *dev) {
    const struct pwm_inputs_config *cfg = dev->config;
    return cfg->num_inputs;
}

#define PWM_INPUTS_DEVICE(index) \
\
static const struct pwm_dt_spec pwm_inputs_##index[] = { \
    DT_INST_FOREACH_CHILD_SEP(index, PWM_DT_SPEC_GET, (,)) \
}; \
\
static const struct pwm_inputs_config pwm_inputs_cfg_##index = { \
    .num_inputs = ARRAY_SIZE(pwm_inputs_##index), \
    .inputs = pwm_inputs_##index, \
}; \
\
DEVICE_DT_INST_DEFINE(index, NULL, NULL, NULL, \
    &pwm_inputs_cfg_##index, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEVICE, NULL);

DT_INST_FOREACH_STATUS_OKAY(PWM_INPUTS_DEVICE);
