#include <unity.h>
#include <app/filter.h>

typedef struct {
    float input1;
    float input2;
    float alpha;
    float expected_output;
} complimentary_test;

void test_filter_complimentary_should_set_alpha_to_one_if_larger_than_one(void) {
    complimentary_test test_input = {
        .input1 = 2.0,
        .input2 = 1.0,
        .alpha = 2.0,
        .expected_output = 2.0,
    };
    float test_output;
    filter_complimentary(&test_input.input1, &test_input.input2, &test_output, 1, test_input.alpha);
    TEST_ASSERT_FLOAT_WITHIN(0.01, test_input.expected_output, test_output);
}

void test_filter_complimentary_should_set_alpha_to_zero_if_less_than_zero(void) {
    complimentary_test test_input = {
        .input1 = 2.0,
        .input2 = 3.0,
        .alpha = -2.0,
        .expected_output = 3.0,
    };
    float test_output;
    filter_complimentary(&test_input.input1, &test_input.input2, &test_output, 1, test_input.alpha);
    TEST_ASSERT_FLOAT_WITHIN(0.01, test_input.expected_output, test_output);
}

void test_filter_complimentary_should_return_expected_value(void) {
    complimentary_test test_input = {
        .input1 = 2.0,
        .input2 = 3.0,
        .alpha = 0.5,
        .expected_output = 2.5,
    };
    float test_output;
    filter_complimentary(&test_input.input1, &test_input.input2, &test_output, 1, test_input.alpha);
    TEST_ASSERT_FLOAT_WITHIN(0.01, test_input.expected_output, test_output);
    test_input.alpha = 0.333;
    test_input.input2 = 3.0;
    test_input.input1 = 1.0;
    test_input.expected_output = 2.333;
    filter_complimentary(&test_input.input1, &test_input.input2, &test_output, 1, test_input.alpha);
    TEST_ASSERT_FLOAT_WITHIN(0.01, test_input.expected_output, test_output);
}

void test_filter_complimentary_should_return_expected_value_for_all_terms(void) {
    complimentary_test test_input1 = {
        .input1 = 2.0,
        .input2 = 3.0,
        .alpha = 0.5,
        .expected_output = 2.5,
    };
    float test_inputs1[] = { test_input1.input1, test_input1.input1 };
    float test_inputs2[] = { test_input1.input2, test_input1.input2 };
    float test_outputs[2];

    filter_complimentary(test_inputs1, test_inputs2, test_outputs, 2, test_input1.alpha);
    TEST_ASSERT_FLOAT_WITHIN(0.01, test_input1.expected_output, test_outputs[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.01, test_input1.expected_output, test_outputs[1]);
}
