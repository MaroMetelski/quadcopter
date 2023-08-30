#ifndef __MOTORS_H__
#define __MOTORS_H__

#include <stdint.h>
#include <stdbool.h>

enum motor {
    MOTOR_FRONT_LEFT,
    MOTOR_FRONT_RIGHT,
    MOTOR_BACK_LEFT,
    MOTOR_BACK_RIGHT,
    MOTOR_LAST,
};

struct motor_config {
    /* Signal (PWM) channel to be assigned to certain motor. */
    uint8_t channel;
    uint32_t max_throttle;
};

bool motor_set(enum motor motor, uint32_t throttle);
bool motor_configure(enum motor motor, struct motor_config *cfg);
enum motor motor_from_string(char *str);

#endif  // __MOTORS_H__
