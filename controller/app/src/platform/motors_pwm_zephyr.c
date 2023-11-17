#include <zephyr/device.h>
#include <drivers/pwm_motors.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(motors_pwm_ll, LOG_LEVEL_INF);

BUILD_ASSERT(DT_NODE_EXISTS(DT_ALIAS(pwm_motors)), "No pwm_motors device found!");
#define DT_PWM_MOTORS DT_ALIAS(pwm_motors)

static const struct device *pwm_motors = DEVICE_DT_GET(DT_PWM_MOTORS);

bool motors_pwm_set(uint8_t channel, uint32_t period, uint32_t pulse)
{
    int rc = pwm_motors_set(pwm_motors, channel, period, pulse, 0);
    if (0 != rc) {
        LOG_ERR("Failed to set motors PWM: %d", rc);
        return false;
    }
    return true;
}
