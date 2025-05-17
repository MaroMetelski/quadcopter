#ifndef __MOTORS_PWM_H_
#define __MOTORS_PWM_H__

#include <stdbool.h>
#include <stdint.h>

bool motors_pwm_set(uint8_t channel, uint32_t period, uint32_t pulse);

#endif  // __MOTORS_PWM_H__
