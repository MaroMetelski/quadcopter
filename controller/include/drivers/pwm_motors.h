#ifndef __PWM_MOTORS_H__
#define __PWM_MOTORS_H__

#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

int pwm_motors_set(const struct device *dev, uint8_t channel,
    uint32_t period, uint32_t pulse, pwm_flags_t flags);

#endif  // __PWM_MOTORS_H__
