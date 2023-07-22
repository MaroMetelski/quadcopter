#include <stdbool.h>
#include <stdlib.h>
#include <tgmath.h>
#include <string.h>
#include <app/logging.h>
#include <app/input.h>
#include "input_pwm.h"

APP_LOG_MODULE_REGISTER(input, APP_LOG_LEVEL_DBG);

struct input_channel_data {
    struct input_channel_config config;
    struct input_channel_pwm_calib calib;
    bool configured;
    bool calibrated_min;
    bool calibrated_max;
};

static struct input_channel_data channels[CHANNEL_FUNC_LAST];

static const char *input_channel_names[] = {
    [CHANNEL_PITCH] = "pitch",
    [CHANNEL_ROLL] = "roll",
    [CHANNEL_YAW] = "yaw",
    [CHANNEL_THROTTLE] = "throttle",
};

enum input_channel input_channel_from_string(char *str) {
    for (int i = 0; i < CHANNEL_FUNC_LAST; i++) {
        if (strcmp(str, input_channel_names[i]) == 0) {
            return (enum input_channel)i;
        }
    }
    return CHANNEL_FUNC_LAST;
}

static bool is_channel_valid(enum input_channel ch)  {
    if (ch >= CHANNEL_FUNC_LAST) {
        return false;
    }
    return true;
}

static bool is_channel_configured(enum input_channel ch) {
    return channels[ch].configured;
}

static bool is_channel_calibrated(enum input_channel ch) {
    return channels[ch].calibrated_min && channels[ch].calibrated_max;
}

bool input_configure_channel(enum input_channel ch, struct input_channel_config *cfg) {
    if (!is_channel_valid(ch)) {
        APP_LOG_ERR("Invalid channel");
        return false;
    }
    channels[ch].config = *cfg;
    channels[ch].configured = true;
    return true;
}

bool input_calibrate_channel_max(enum input_channel ch) {
    if (!is_channel_configured(ch) || !is_channel_valid(ch)) {
        APP_LOG_ERR("Invalid channel or channel not configured");
        return false;
    }
    uint64_t data;
    int ret = input_pwm_get_channel_duty_usec(&data, channels[ch].config.channel);
    if (ret < 0) {
        return false;
    }
    channels[ch].calib.max_dc = (uint32_t)data;
    APP_LOG_DBG("calibration max duty cycle %d", channels[ch].calib.max_dc);
    channels[ch].calibrated_max = true;
    return true;
}

bool input_calibrate_channel_min(enum input_channel ch) {
    if (!is_channel_configured(ch) || !is_channel_valid(ch)) {
        APP_LOG_ERR("Invalid channel or channel not configured");
        return false;
    }
    uint64_t data;
    int ret = input_pwm_get_channel_duty_usec(&data, channels[ch].config.channel);
    if (ret < 0) {
        return false;
    }
    channels[ch].calib.min_dc = (uint32_t)data;
    APP_LOG_DBG("calibration max duty cycle %d", channels[ch].calib.min_dc);
    channels[ch].calibrated_min = true;
    return true;
}

static float input_convert(enum input_channel ch, uint32_t raw_dc) {
    // handle channels calibrated in reverse
    int min_dc = (int)channels[ch].calib.min_dc;
    int max_dc = (int)channels[ch].calib.max_dc;
    float min_value = channels[ch].config.min_val;
    float max_value = channels[ch].config.max_val;
    // we have two points (max_dc, max_value) and (min_dc, min_value), use linear equation.
    float slope = (max_value - min_value) / (float)(max_dc - min_dc);
    // max_val = slope * max_dc + offset => b = max_val - (slope * max_dc)
    float offset = max_value - (slope * max_dc);
    float value = slope * raw_dc + offset;
    // make sure value stays in bounds
    if (max_value >= min_value) {
        value = value > max_value ? max_value : value;
        value = value < min_value ? min_value : value;
    } else {  // max_value < min_value ('reversed' channel)
        value = value > min_value ? min_value : value;
        value = value < max_value ? max_value : value;
    }
    return value;
}

float input_get_channel_value(enum input_channel ch) {
    if (!is_channel_valid(ch) || !is_channel_configured(ch) || !is_channel_calibrated(ch)) {
        APP_LOG_ERR("Invalid channel or channel not calibrated");
        return 0;
    }
    uint64_t data;
    int ret = input_pwm_get_channel_duty_usec(&data, channels[ch].config.channel);
    if (ret < 0) {
        return 0;
    }
    return input_convert(ch, (uint32_t)data);
}