#include <unity.h>
#include <app/mixer.h>

static struct mixer test_mixer[MOTOR_LAST] = {
    {1.0, 1.0, 1.0, 1.0},
    {-1.0, -1.0, -1.0, -1.0},
    {0.0, 0.0, 0.0, 0.0},
    {1.0, 0.0, 1.0, 0.0},
};

/* Depends on values from test_mixer */
#define MIXER_RESULT_FRONT_LEFT(in) \
    ((uint32_t)(1.0 * in.throttle + 1.0 * in.pitch + 1.0 * in.roll + 1.0 * in.yaw))
#define MIXER_RESULT_FRONT_RIGHT(in) \
    ((uint32_t)(-1.0 * in.throttle + -1.0 * in.pitch + -1.0 * in.roll + -1.0 * in.yaw))
#define MIXER_RESULT_BACK_LEFT(in) \
    ((uint32_t)(0.0 * in.throttle + 0.0 * in.pitch + 0.0 * in.roll + 0.0 * in.yaw))
#define MIXER_RESULT_BACK_RIGHT(in) \
    ((uint32_t)(1.0 * in.throttle + 0.0 * in.pitch + 1.0 * in.roll + 0.0 * in.yaw))


void setUp() {
    mixer_configure(test_mixer, MOTOR_LAST);
}

void test_mixer_configure_should_return_false_when_num_motors_is_to_big() {
    TEST_ASSERT_FALSE(mixer_configure(test_mixer, MOTOR_LAST + 1));
}

void test_mixer_should_return_sum_of_mixer_values_multiplied_by_input() {
    struct mixer input = {
        .pitch = 1.0,
        .roll = 2.0,
        .throttle = 3.0,
        .yaw = 4.0,
    };
    uint32_t result_fl = mixer_get_motor_throttle(MOTOR_FRONT_LEFT, &input);
    uint32_t result_fr = mixer_get_motor_throttle(MOTOR_FRONT_RIGHT, &input);
    uint32_t result_bl = mixer_get_motor_throttle(MOTOR_BACK_LEFT, &input);
    uint32_t result_br = mixer_get_motor_throttle(MOTOR_BACK_RIGHT, &input);
    TEST_ASSERT_EQUAL(MIXER_RESULT_FRONT_LEFT(input), result_fl);
    TEST_ASSERT_EQUAL(MIXER_RESULT_FRONT_RIGHT(input), result_fr);
    TEST_ASSERT_EQUAL(MIXER_RESULT_BACK_LEFT(input), result_bl);
    TEST_ASSERT_EQUAL(MIXER_RESULT_BACK_RIGHT(input), result_br);
}
