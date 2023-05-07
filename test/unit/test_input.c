#include <unity.h>

#include <app/input.h>
#include "mock_input_pwm.h"

#define TEST_CASE(...)
#define TEST_RANGE(...)

#define TEST_PWM_CHANNEL 0
#define TEST_CHANNEL 0

static bool successfully_configure_test_input(enum input_channel channel, float min_val, float max_val) {
    struct input_channel_config cfg = {
        .channel = TEST_PWM_CHANNEL,
        .min_val = min_val,
        .max_val = max_val,
    };
    TEST_ASSERT_TRUE(input_configure_channel(channel, &cfg));
}

static void channel_return_dc_thru_ptr(uint64_t *data) {
    input_pwm_get_channel_duty_usec_ExpectAndReturn(NULL, TEST_PWM_CHANNEL, true);
    input_pwm_get_channel_duty_usec_IgnoreArg_data();
    input_pwm_get_channel_duty_usec_ReturnThruPtr_data(data);
}

static void successfully_calibrate_min(enum input_channel channel, int min_dc) {
    uint64_t raw_dc = min_dc;
    channel_return_dc_thru_ptr(&raw_dc);
    TEST_ASSERT_TRUE(input_calibrate_channel_min(channel));
}

static void successfully_calibrate_max(enum input_channel channel, int min_dc) {
    uint64_t raw_dc = min_dc;
    channel_return_dc_thru_ptr(&raw_dc);
    TEST_ASSERT_TRUE(input_calibrate_channel_max(channel));
}

void setUp(void) {
    for (int i = 0; i < CHANNEL_FUNC_LAST; i++) {
        successfully_configure_test_input(i, 0, 0);
    }
}

void test_configure_should_fail_when_channel_is_invalid(void) {
    struct input_channel_config cfg = {0, };
    TEST_ASSERT_FALSE(input_configure_channel(CHANNEL_FUNC_LAST, &cfg));
}

void test_input_should_return_zero_when_channel_is_not_calibrated(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.0, input_get_channel_value(TEST_CHANNEL));
}

void test_input_should_return_zero_when_channel_is_invalid(void) {
    TEST_ASSERT_EQUAL_FLOAT(0.0, input_get_channel_value(CHANNEL_FUNC_LAST));
}

void test_input_should_return_zero_when_pwm_capture_fails(void) {
    successfully_configure_test_input(TEST_CHANNEL, 0, 1);
    successfully_calibrate_min(TEST_CHANNEL, 0);
    successfully_calibrate_max(TEST_CHANNEL, 1);
    input_pwm_get_channel_duty_usec_ExpectAndReturn(NULL, TEST_PWM_CHANNEL, -1);
    input_pwm_get_channel_duty_usec_IgnoreArg_data();
    TEST_ASSERT_EQUAL_FLOAT(0.0, input_get_channel_value(TEST_CHANNEL));
}

TEST_CASE(0, 200, 400, -20, 20, 500, 20) // value out of upper bounds
TEST_CASE(0, 200, 400, -20, 20, 100, -20) // value out of lower bounds
TEST_CASE(0, 200, 400, 20, -20, 250, 10) // reversed min / max values
TEST_CASE(0, 400, 200, -20, 20, 250, 10) // reversed pwm signal
TEST_CASE(1, 200, 400, -20, 20, 250, -10)  // non reversed
TEST_CASE(0, 100, 200, 0, 10, 150, 5) // non reversed, only positive values
void test_input_channel_behavior(
    int channel,
    int min_dc, int max_dc,
    float min_val, float max_val,
    uint64_t test_dc, float expected_val)
{
    successfully_configure_test_input(channel, min_val, max_val);
    successfully_calibrate_min(channel, min_dc);
    successfully_calibrate_max(channel, max_dc);
    uint64_t raw_dc = test_dc;
    channel_return_dc_thru_ptr(&raw_dc);
    float val = input_get_channel_value(channel);
    TEST_ASSERT_FLOAT_WITHIN(0.1, expected_val, val);
}
