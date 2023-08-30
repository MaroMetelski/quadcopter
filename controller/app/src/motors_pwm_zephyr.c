#include <zephyr/device.h>
#include <drivers/pwm_motors.h>

BUILD_ASSERT(DT_NODE_EXISTS(DT_ALIAS(pwm_motors)), "No pwm_motors device found!");
#define DT_PWM_MOTORS DT_ALIAS(pwm_motors)

static const struct device *pwm_motors = DEVICE_DT_GET(DT_PWM_MOTORS);

bool motors_pwm_set(uint8_t channel, uint32_t period, uint32_t pulse)
{
    if (0 != pwm_motors_set(pwm_motors, channel, period, pulse, 0)) {
        return false;
    }
    return true;
}
