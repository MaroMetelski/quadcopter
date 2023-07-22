#include <zephyr/device.h>
#include <drivers/pwm_inputs.h>

#include "input_pwm.h"

BUILD_ASSERT(DT_NODE_EXISTS(DT_ALIAS(pwm_inputs)), "No pwm_inputs device found!");
#define DT_PWM_INPUTS DT_ALIAS(pwm_inputs)

static const struct device *pwm_inputs = DEVICE_DT_GET(DT_PWM_INPUTS);

int input_pwm_get_channel_duty_usec(uint64_t *data, uint8_t channel) {
    return pwm_inputs_get_channel_duty_usec(pwm_inputs, data, channel);
}

int input_pwm_get_num_channels(void) {
    return pwm_inputs_get_num_channels(pwm_inputs);
}
