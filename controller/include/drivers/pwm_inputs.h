#ifndef __PWM_INPUTS_H__
#define __PWM_INPUTS_H__

#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <stdint.h>

/** @brief Configure single channel capture for use with a callback.
 *
 * @param dev 'pwm-inputs' compatible device.
 * @param channel Channel number to read.
 * @param flags PWM_CAPTURE_TYPE_* and PWM_CAPTURE_MODE_* flags.
 * @param cb Capture callback.
 * @param user_data User data passed to capture callback.
*/
int pwm_inputs_configure_capture(const struct device *dev, uint8_t channel, pwm_flags_t flags,
    pwm_capture_callback_handler_t cb, void *user_data);

/** @brief Enable configured capture.
 *
 * @param dev 'pwm-inputs' compatible device.
 * @param channel Channel number to read.
*/
int pwm_inputs_enable_capture(const struct device *dev, uint8_t channel);

/** @brief Disable capture.
 *
 * @param dev 'pwm-inputs' compatible device.
 * @param channel Channel number to read.
*/
int pwm_inputs_disable_capture(const struct device *dev, uint8_t channel);

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
