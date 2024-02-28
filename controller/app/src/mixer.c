#include <controller/mixer.h>
#include <string.h>

static const struct mixer mixer_quad_x_default[MOTOR_LAST] = {
    [MOTOR_FRONT_LEFT] = {
        .throttle = 1.0,
        .pitch = -1.0,
        .roll = 1.0,
        .yaw = 1.0,
    },
    [MOTOR_FRONT_RIGHT] = {
        .throttle = 1.0,
        .pitch = 1.0,
        .roll = 1.0,
        .yaw = -1.0,
    },
    [MOTOR_BACK_LEFT] = {
        .throttle = 1.0,
        .pitch = -1.0,
        .roll = -1.0,
        .yaw = 1.0,
    },
    [MOTOR_BACK_LEFT] = {
        .throttle = 1.0,
        .pitch = 1.0,
        .roll = 1.0,
        .yaw = 1.0,
    },
    [MOTOR_BACK_RIGHT] = {
        .throttle = 1.0,
        .pitch = 1.0,
        .roll = -1.0,
        .yaw = -1.0,
    },
};

struct mixer current_mixer[MOTOR_LAST];

bool mixer_configure(const struct mixer *mixer, uint8_t num_motors) {
    if (num_motors > MOTOR_LAST) {
        return false;
    }
    memcpy(current_mixer, mixer, num_motors * sizeof(struct mixer));
    return true;
}

uint32_t mixer_get_motor_throttle(enum motor motor, struct mixer *in) {
    float throttle_fragment = in->throttle * current_mixer[motor].throttle;
    float pitch_fragment = in->pitch * current_mixer[motor].pitch;
    float roll_fragment = in->roll * current_mixer[motor].roll;
    float yaw_fragment = in->yaw * current_mixer[motor].yaw;
    uint32_t throttle = (uint32_t)(throttle_fragment
                                    + pitch_fragment
                                    + roll_fragment
                                    + yaw_fragment);
    return throttle;
}

void mixer_configure_default(void) {
    (void)mixer_configure(mixer_quad_x_default, MOTOR_LAST);
}
