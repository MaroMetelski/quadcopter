#ifndef __SBUS_DECODER_H__
#define __SBUS_DECODER_H__

#include <stdint.h>
#include <stdbool.h>

/** Number of regular SBus channels (excluding binary channel 16 and 17). */
#define SBUS_DATA_CHANNELS 16
/** Total number of SBus channels (including binary channels). */
#define SBUS_TOT_CHANNELS  SBUS_DATA_CHANNELS + 2
#define SBUS_FRAME_SIZE    25

#define SBUS_HEADER       0x0F
#define SBUS_FOOTER       0x00

/** SBus frame data structure. */
struct sbus_frame {
    uint16_t channels[SBUS_DATA_CHANNELS];
    bool ch17;
    bool ch18;
    bool frame_lost;
    bool failsafe;
};

/** @brief Decode raw SBus frame.
 *
 * Decodes raw SBus frame to an output structure. When skip_header is set,
 * The first byte will be interpreted as data. When skip_footer is set, the
 * function will not try to verify the last byte as footer.
 *
 * @param data Raw SBus frame.
 * @param size Size of the input data.
 * @param no_header Set if header is not present in the data.
 * @param no_footer Set if footer is not present in the data.
 * @param frame Pointer to output structure for decoded data.
 *
 * @return '0' when successful.
 * @return '-1' when frame decoding fails.
*/
int sbus_decode_frame(const uint8_t *data, uint8_t size, bool no_header,
    bool no_footer, struct sbus_frame *frame);

#endif  // __SBUS_DECODER_H__
