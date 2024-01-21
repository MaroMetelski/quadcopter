#ifndef __INPUT_PWM_H__
#define __INPUT_PWM_H__

#include <stdint.h>

typedef void (*input_pwm_capture_callback)(uint8_t channel, uint64_t period_usec,
    uint32_t duty_usec, int status);

int input_pwm_start_capture(uint8_t channel, input_pwm_capture_callback cb);
int input_pwm_stop_capture(uint8_t channel);
int input_pwm_get_channel_duty_usec(uint64_t *data, uint8_t channel);
int input_pwm_get_num_channels(void);

#endif  // __INPUT_PWM_H__
