#include <controller/motors.h>
#include <controller/logging.h>
#include <string.h>
#include <controller/low_level/motors_pwm.h>

APP_LOG_MODULE_REGISTER(motors, APP_LOG_LEVEL_DBG);

#define MOTOR_PWM_FREQ_HZ 2000UL
#define MOTOR_PWM_PERIOD_NSEC (1000000000UL / MOTOR_PWM_FREQ_HZ)

struct motor_data {
    struct motor_config config;
    bool configured;
};

static struct motor_data motors[MOTOR_LAST];

static bool is_motor_valid(enum motor motor) {
    if (motor >= MOTOR_LAST) {
        return false;
    }
    return true;
}

static const char *motor_names[] = {
    [MOTOR_FRONT_LEFT] = "motor_fl",
    [MOTOR_FRONT_RIGHT] = "motor_fr",
    [MOTOR_BACK_LEFT] = "motor_bl",
    [MOTOR_BACK_RIGHT] = "motor_br",
};

enum motor motor_from_string(char *str) {
    for (int i = 0; i < MOTOR_LAST; i++) {
        if (strcmp(str, motor_names[i]) == 0) {
            return (enum motor)i;
        }
    }
    return MOTOR_LAST;
}

static uint32_t throttle_to_duty(enum motor motor, uint32_t throttle) {
    struct motor_data *current_motor = &motors[motor];
    uint32_t max_throttle = current_motor->config.max_throttle;
    uint32_t max_duty = MOTOR_PWM_PERIOD_NSEC;
    if (max_throttle > max_duty) {
        return max_duty;
    }
    uint32_t step_size = max_duty / max_throttle;
    return throttle * step_size;
}

bool motor_configure(enum motor motor, struct motor_config *cfg) {
    if (!is_motor_valid(motor)) {
        APP_LOG_ERR("Invalid motor");
        return false;
    }
    motors[motor].config = *cfg;
    motors[motor].configured = true;
    return true;
}

bool motor_set(enum motor motor, uint32_t throttle) {
    if (!is_motor_valid(motor)) {
        APP_LOG_ERR("Invalid motor");
        return false;
    }
    uint32_t duty = throttle_to_duty(motor, throttle);
    struct motor_data *current_motor = &motors[motor];
    return motors_pwm_set(current_motor->config.channel, MOTOR_PWM_PERIOD_NSEC, duty);
}
