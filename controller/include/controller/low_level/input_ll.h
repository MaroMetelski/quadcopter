#ifndef __INPUT_LL_H__
#define __INPUT_LL_H__

#include <stdint.h>

/** Callback that will be called upon reception of a full frame (all channels). */
typedef void (*input_ll_frame_callback)(uint16_t *data, uint8_t size);
/** Callback that will be called upon reception of a data on a specified channel. */
typedef void (*input_ll_channel_callback)(uint8_t channel, uint16_t *data, uint8_t size);

struct input_ll_api {
    int (*set_frame_callback)(input_ll_frame_callback cb);
    int (*set_channel_callback)(input_ll_channel_callback cb);
    int (*start)(void);
    int (*stop)(void);
};

#endif  // __INPUT_LL_H__
