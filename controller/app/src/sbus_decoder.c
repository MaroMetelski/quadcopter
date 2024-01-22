#include <controller/sbus_decoder.h>

#define CHANNEL_BYTES   22
#define FAILSAFE_BIT    (1 << 3)
#define FRAME_LOST_BIT  (1 << 2)
#define CH18_BIT        (1 << 1)
#define CH17_BIT        1

/** @brief Decode 16 11-bit channels from 22 input bytes. */
static void sbus_decode_channels(uint16_t *out, const uint8_t *in) {
    // amount of bits remaining for current number (out[j])
    uint8_t remaining_cur = 11;
    // amount of bits remaining for next number (out[j + 1])
    uint8_t remaining_next = 11;

    for (int i = 0, j = 0; i < CHANNEL_BYTES; i++) {
        // how many bits in this byte will go towards out[j]
        uint8_t bits_to_cur = remaining_cur > 8 ? 8 : remaining_cur;
        // how many bits in this byte will go towards out[j + 1]
        uint8_t bits_to_next = remaining_cur > 8 ? 0 : 8 - remaining_cur;
        // mask for lower nibble in this byte
        uint8_t lower_mask = (1 << bits_to_cur) - 1;
        // mask for upper nibble in this byte
        uint8_t upper_mask = ((1 << bits_to_next) - 1) << (8 - bits_to_next);
        if (bits_to_cur) {
            // extract lower nibble that contributes to out[j]
            uint16_t lower_nibble = (in[i] & lower_mask);
            // add lower nibble to position in out[j + 1]
            out[j] |= lower_nibble << (11 - remaining_cur);
        }
        if (bits_to_next) {
            // extract upper nibble that contributes to out[j + 1]
            uint16_t upper_nibble = ((in[i] & upper_mask) >> (8 - bits_to_next));
            // add upper nibble to position in out[j + 1]
            out[j + 1] |= upper_nibble << (11 - remaining_next);
        }
        remaining_cur -= bits_to_cur;
        remaining_next -= bits_to_next;
        // Completed "current". "next" now becomes "current" and is partially complete.
        if (remaining_cur == 0) {
            remaining_cur = remaining_next;
            remaining_next = 11;
            j++;
        }
    }
}

int sbus_decode_frame(const uint8_t *data, uint8_t size, bool no_header,
    bool no_footer, struct sbus_frame *frame)
{
    uint8_t expected_size = SBUS_FRAME_SIZE;
    uint8_t cur_pos = 0;

    if (no_header) expected_size--;
    if (no_footer) expected_size--;

    /* Verify frame size. */
    if (size != expected_size) {
        return -1;
    }

    /* Verify header character. */
    if (!no_header) {
        if (data[cur_pos++] != SBUS_HEADER) {
            return -1;
        }
    }

    sbus_decode_channels(frame->channels, &data[cur_pos]);
    cur_pos += CHANNEL_BYTES;

    frame->ch17 = (bool)(data[cur_pos] & CH17_BIT);
    frame->ch18 = (bool)(data[cur_pos] & CH18_BIT);
    frame->failsafe = (bool)(data[cur_pos] & FAILSAFE_BIT);
    frame->frame_lost = (bool)(data[cur_pos] & FRAME_LOST_BIT);

    if (!no_footer) {
        if (data[++cur_pos] != SBUS_FOOTER) {
            return -1;
        }
    }
    return 0;
}
