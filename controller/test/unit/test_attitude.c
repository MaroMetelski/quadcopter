#include <unity.h>
#include <controller/attitude.h>
#include "mock_quaternion.h"
#include "mock_calc.h"

void test_quaternion_to_euler_returns_all_zero_angles_when_quaternion_is_unit() {
    float test_quaternion[4] = {1, 0, 0, 0};
    float expected_angles[3] = {0, 0, 0};
    float test_angles[3] = {0,};
    attitude_quaternion_to_euler(test_quaternion, test_angles);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected_angles, test_angles, 3);
}

void test_quaternion_to_euler_expected_pitch_between_0_and_90() {
    float test_quaternion[4] = {0.92, 0, 0.39, 0};
    float test_angles[3] = {0,};
    attitude_quaternion_to_euler(test_quaternion, test_angles);
    TEST_ASSERT_EQUAL_FLOAT(0, test_angles[1]);
    TEST_ASSERT_EQUAL_FLOAT(0, test_angles[2]);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 0.8, test_angles[0]);
}

void test_quaternion_to_euler_expected_roll_between_0_and_90() {
    float test_quaternion[4] = {0.92, 0.39, 0, 0};
    float test_angles[3] = {0,};
    attitude_quaternion_to_euler(test_quaternion, test_angles);
    TEST_ASSERT_EQUAL_FLOAT(0, test_angles[0]);
    TEST_ASSERT_EQUAL_FLOAT(0, test_angles[2]);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 0.8, test_angles[1]);
}

void test_quaternion_to_euler_expected_yaw_between_0_and_90() {
    float test_quaternion[4] = {0.92, 0, 0, 0.39};
    float test_angles[3] = {0,};
    attitude_quaternion_to_euler(test_quaternion, test_angles);
    TEST_ASSERT_EQUAL_FLOAT(0, test_angles[0]);
    TEST_ASSERT_EQUAL_FLOAT(0, test_angles[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 0.8, test_angles[2]);
}
