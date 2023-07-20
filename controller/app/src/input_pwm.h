#ifndef __INPUT_PWM_H__
#define __INPUT_PWM_H__

#include <stdint.h>

int input_pwm_get_channel_duty_usec(uint64_t *data, uint8_t channel);
int input_pwm_get_num_channels(void);

#endif  // __INPUT_PWM_H__
