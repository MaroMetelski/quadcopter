#ifndef __PWM_INPUTS_H__
#define __PWM_INPUTS_H__

#include <zephyr/device.h>
#include <stdint.h>

/** @brief Read single channel duty cycle in microseconds in a blocking manner.
 *
 * @param dev 'pwm-inputs' compatible device.
 * @param data Output data pointer.
 * @param channel Channel number to read.
 */
int pwm_inputs_get_channel_duty_usec(
    const struct device *dev, uint64_t *data, uint8_t channel);

/** @brief Get number of available channels for that device.
 *
 * @param dev 'pwm-inputs' compatible device.
 */
int pwm_inputs_get_num_channels(const struct device *dev);

#endif  // __PWM_INPUTS_H__
