#ifndef __INPUT_H__
#define __INPUT_H__

#include <stdint.h>
#include <stdbool.h>

enum input_channel {
    CHANNEL_PITCH,
    CHANNEL_ROLL,
    CHANNEL_YAW,
    CHANNEL_THROTTLE,
    CHANNEL_FUNC_LAST
};

struct input_channel_pwm_calib {
    uint32_t max_dc;
    uint32_t min_dc;
};

struct input_channel_config {
    uint8_t channel;
    float min_val;
    float max_val;
};

enum input_channel input_channel_from_string(char *str);
bool input_configure_channel(enum input_channel ch, struct input_channel_config *cfg);
bool input_calibrate_channel_max(enum input_channel ch);
bool input_calibrate_channel_min(enum input_channel ch);
float input_get_channel_value(enum input_channel ch);

#endif  // __INPUT_H__
