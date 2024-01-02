#include <zephyr/device.h>
#include <drivers/pwm_inputs.h>
#include <controller/low_level/input_pwm.h>

BUILD_ASSERT(DT_NODE_EXISTS(DT_ALIAS(pwm_inputs)), "No pwm_inputs device found!");
#define DT_PWM_INPUTS DT_ALIAS(pwm_inputs)

#define CAPTURE_FLAGS (PWM_CAPTURE_MODE_CONTINUOUS | PWM_CAPTURE_TYPE_BOTH)
#define MAX_INPUT_CHANNELS 10

static const struct device *pwm_inputs = DEVICE_DT_GET(DT_PWM_INPUTS);

static struct capture_channel {
    uint8_t channel_idx;
    input_pwm_capture_callback cb;
    bool active;
} capture_channels[MAX_INPUT_CHANNELS];

static void internal_capture_callback(const struct device *dev, uint32_t channel,
    uint32_t period_cycles, uint32_t pulse_cycles, int status, void *user_data)
{
    uint64_t duty_us, period_us;

    if (0 != pwm_cycles_to_usec(dev, channel, period_cycles, &period_us)) {
        return;
    }
    if (0 != pwm_cycles_to_usec(dev, channel, pulse_cycles, &duty_us)) {
        return;
    }

    struct capture_channel *current = (struct capture_channel *)user_data;
    if (current->active && current->cb) {
        current->cb(current->channel_idx, period_us, duty_us, status);
    }
}

int input_pwm_start_capture(uint8_t channel, input_pwm_capture_callback cb)
{
    int ret;

    if (channel > ARRAY_SIZE(capture_channels)) {
        return -EINVAL;
    }
    /* Pass the top-level channel as user data so correct callback can be called.
       The callback itself receives the underlying PWM channel which is of no use here. */
    capture_channels[channel].channel_idx = channel;
    capture_channels[channel].cb = cb;

    ret = pwm_inputs_configure_capture(pwm_inputs, channel, CAPTURE_FLAGS,
        internal_capture_callback, (void *)&capture_channels[channel]);
    if (ret < 0) {
        return ret;
    }
    ret = pwm_inputs_enable_capture(pwm_inputs, channel);
    if (ret == 0) {
        capture_channels[channel].active = true;
    }
    return ret;
}

int input_pwm_stop_capture(uint8_t channel)
{
    capture_channels[channel].active = false;
    return pwm_inputs_disable_capture(pwm_inputs, channel);
}

int input_pwm_get_channel_duty_usec(uint64_t *data, uint8_t channel) {
    return pwm_inputs_get_channel_duty_usec(pwm_inputs, data, channel);
}

int input_pwm_get_num_channels(void) {
    return pwm_inputs_get_num_channels(pwm_inputs);
}
