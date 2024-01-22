#include <unity.h>
#include <stdint.h>
#include <string.h>
#include "controller/sbus_decoder.h"

#define TEST_CASE(...)

/** Test packet corresponding to 16 `test_out_channels` and ch17, ch18, failsafe
 * and frame_lost all set to '1'. */
static const uint8_t test_in_packet[25] = {
    0x0f, 0b11001010, 0b01010110, 0b10110110, 0b10110010,
    0b10010101, 0b10101101, 0b01101100, 0b01100101, 0b00101011,
    0b01011011, 0b11011001, 0b11001010, 0b01010110, 0b10110110,
    0b10110010, 0b10010101, 0b10101101, 0b01101100, 0b01100101,
    0b00101011, 0b01011011, 0b11011001, 0b00001111, 0x00
};

static const uint16_t test_out_channels[16] = {
    1738, 1738, 1738, 1738, 1738, 1738, 1738, 1738,
    1738, 1738, 1738, 1738, 1738, 1738, 1738, 1738
};

void test_sbus_decoder_correctly_decodes_all_channels_without_skipping_header_and_footer()
{
    struct sbus_frame frame = {0};

    int rc = sbus_decode_frame(test_in_packet, sizeof(test_in_packet), false, false, &frame);

    TEST_ASSERT_EQUAL_MEMORY_ARRAY(test_out_channels, frame.channels, 1, 16);
    TEST_ASSERT_TRUE(frame.failsafe);
    TEST_ASSERT_TRUE(frame.frame_lost);
    TEST_ASSERT_TRUE(frame.ch17);
    TEST_ASSERT_TRUE(frame.ch18);
    TEST_ASSERT_EQUAL_INT(0, rc);
}

void test_sbus_decoder_correctly_decodes_all_channels_skipping_header_only()
{
    struct sbus_frame frame = {0};

    int rc = sbus_decode_frame(
        &test_in_packet[1], sizeof(test_in_packet) - 1, true, false, &frame);

    TEST_ASSERT_EQUAL_MEMORY_ARRAY(test_out_channels, frame.channels, 1, 16);
    TEST_ASSERT_TRUE(frame.failsafe);
    TEST_ASSERT_TRUE(frame.frame_lost);
    TEST_ASSERT_TRUE(frame.ch17);
    TEST_ASSERT_TRUE(frame.ch18);
    TEST_ASSERT_EQUAL_INT(0, rc);
}

void test_sbus_decoder_correctly_decodes_all_channels_skipping_footer_only()
{
    struct sbus_frame frame = {0};

    int rc = sbus_decode_frame(
        test_in_packet, sizeof(test_in_packet) - 1, false, true, &frame);

    TEST_ASSERT_EQUAL_MEMORY_ARRAY(test_out_channels, frame.channels, 1, 16);
    TEST_ASSERT_TRUE(frame.failsafe);
    TEST_ASSERT_TRUE(frame.frame_lost);
    TEST_ASSERT_TRUE(frame.ch17);
    TEST_ASSERT_TRUE(frame.ch18);
    TEST_ASSERT_EQUAL_INT(0, rc);
}

void test_sbus_decoder_correctly_decodes_all_channels_skipping_footer_and_header()
{
    struct sbus_frame frame = {0};

    int rc = sbus_decode_frame(
        &test_in_packet[1], sizeof(test_in_packet) - 2, true, true, &frame);

    TEST_ASSERT_EQUAL_MEMORY_ARRAY(test_out_channels, frame.channels, 1, 16);
    TEST_ASSERT_TRUE(frame.failsafe);
    TEST_ASSERT_TRUE(frame.frame_lost);
    TEST_ASSERT_TRUE(frame.ch17);
    TEST_ASSERT_TRUE(frame.ch18);
    TEST_ASSERT_EQUAL_INT(0, rc);
}

TEST_CASE(1)
TEST_CASE(0)
void test_sbus_decoder_correctly_decodes_failsafe(int failsafe)
{
    struct sbus_frame frame = {0};
    uint8_t _test_packet[25] = {0};
    memcpy(_test_packet, test_in_packet, 25);
    _test_packet[23] = (0 | (failsafe << 3));

    int rc = sbus_decode_frame(_test_packet, sizeof(_test_packet), false, false, &frame);

    TEST_ASSERT_EQUAL(failsafe, frame.failsafe);
    TEST_ASSERT_EQUAL_INT(0, rc);
}

TEST_CASE(1)
TEST_CASE(0)
void test_sbus_decoder_correctly_decodes_frame_lost(int frame_lost)
{
    struct sbus_frame frame = {0};
    uint8_t _test_packet[25] = {0};
    memcpy(_test_packet, test_in_packet, 25);
    _test_packet[23] = (0 | (frame_lost << 2));

    int rc = sbus_decode_frame(_test_packet, sizeof(_test_packet), false, false, &frame);

    TEST_ASSERT_EQUAL(frame_lost, frame.frame_lost);
    TEST_ASSERT_EQUAL_INT(0, rc);
}

TEST_CASE(1)
TEST_CASE(0)
void test_sbus_decoder_correctly_decodes_ch17(int ch17)
{
    struct sbus_frame frame = {0};
    uint8_t _test_packet[25] = {0};
    memcpy(_test_packet, test_in_packet, 25);
    _test_packet[23] = (0 | (ch17 << 0));

    int rc = sbus_decode_frame(_test_packet, sizeof(_test_packet), false, false, &frame);

    TEST_ASSERT_EQUAL(ch17, frame.ch17);
    TEST_ASSERT_EQUAL_INT(0, rc);
}

TEST_CASE(1)
TEST_CASE(0)
void test_sbus_decoder_correctly_decodes_ch18(int ch18)
{
    struct sbus_frame frame = {0};
    uint8_t _test_packet[25] = {0};
    memcpy(_test_packet, test_in_packet, 25);
    _test_packet[23] = (0 | (ch18 << 1));

    int rc = sbus_decode_frame(_test_packet, sizeof(_test_packet), false, false, &frame);

    TEST_ASSERT_EQUAL(ch18, frame.ch18);
    TEST_ASSERT_EQUAL_INT(0, rc);
}
