#ifndef __INPUT_SBUS_H__
#define __INPUT_SBUS_H__

#include <stdint.h>
#include <controller/low_level/input_ll.h>

/** Set input frame callback for SBus UART. */
int input_sbus_set_frame_callback(input_ll_frame_callback cb);
/** Start input reception using SBus UART. */
int input_sbus_start(void);
/** Stop input reception using SBus UART. */
int input_sbus_stop(void);

#endif  // __INPUT_SBUS_H__
