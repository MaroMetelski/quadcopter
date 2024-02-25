#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <app/logging.h>
#include <controller/input.h>
#include <controller/low_level/input_ll.h>
#include <controller/low_level/input_sbus.h>

APP_LOG_MODULE_REGISTER(input, APP_LOG_LEVEL_DBG);

struct input_channel_data {
    struct input_channel_config config;
    struct input_channel_pwm_calib calib;
    bool configured;
    bool calibrated_min;
    bool calibrated_max;
    uint16_t raw_val;
};

#ifdef CONFIG_APP_INPUT_SBUS_ZEPHYR
struct input_ll_api input_ll  = {
    .set_frame_callback = input_sbus_set_frame_callback,
    .start = input_sbus_start,
    .stop = input_sbus_stop,
};
#endif

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

static void input_ll_callback(uint16_t *data, uint8_t size) {
    for (int i = 0; i < CHANNEL_FUNC_LAST; i++) {
        if (!is_channel_configured(i)) {
            continue;
        }
        channels[i].raw_val = 0;
        uint8_t internal_channel_num = channels[i].config.channel;
        /* This channel is not available in incoming data. */
        if (internal_channel_num >= size) {
            continue;
        }
        channels[i].raw_val = data[internal_channel_num];
    }
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

bool input_get_calibration(
    enum input_channel ch, struct input_channel_pwm_calib *calib)
{
    if (!is_channel_calibrated(ch) || !is_channel_valid(ch)) {
        APP_LOG_ERR("Invalid channel or channel not calibrated");
        return false;
    }
    *calib = channels[ch].calib;
    return true;
}

bool input_set_calibration(
    enum input_channel ch, struct input_channel_pwm_calib *calib)
{
    if (!is_channel_configured(ch) || !is_channel_valid(ch)) {
        APP_LOG_ERR("Invalid channel or channel not configured");
        return false;
    }
    if (!calib) {
        return false;
    }
    channels[ch].calib = *calib;
    channels[ch].calibrated_max = true;
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

uint32_t input_get_channel_raw_value(enum input_channel ch)
{
    if (!is_channel_configured(ch) || !is_channel_valid(ch)) {
        APP_LOG_ERR("Invalid channel or channel not configured");
        return false;
    }
    uint16_t data = channels[ch].raw_val;
    APP_LOG_DBG("channel raw duty cycle: %d", (uint32_t)data);
    return (uint32_t)data;
}

float input_get_channel_value(enum input_channel ch) {
    if (!is_channel_valid(ch) || !is_channel_configured(ch) || !is_channel_calibrated(ch)) {
        APP_LOG_ERR("Invalid channel or channel not calibrated");
        return 0;
    }
    uint16_t data = channels[ch].raw_val;
    return input_convert(ch, (uint32_t)data);
}

bool input_init(void) {
    if (0 != input_ll.set_frame_callback(input_ll_callback)) {
        return false;
    }
    return true;
}

bool input_start(void) {
    if (0 != input_ll.start()) {
        return false;
    }
    return true;
}

bool input_stop(void) {
    if (0 != input_ll.stop()) {
        return false;
    }
    return true;
}
