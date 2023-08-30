#ifndef __MIXER_H__
#define __MIXER_H__

#include <stdint.h>
#include <app/motors.h>

/**
* Motor mixer structure. This structure serves two purposes:
* 1. Configure the influence of each motor on the respective axis. When configuring
* The values should be in range -1.0 to 1.0.
* 2. Input to mixer carrying information about controller output for each axis / throttle.
*/
struct mixer {
    float throttle;
    float pitch;
    float roll;
    float yaw;
};

/** @brief Set predefined or custom mixer array for motors.
 *
 * @param mixer Mixer array.
 * @param num_motors Number of individual motor mixers in mixer array.
 */
bool mixer_configure(const struct mixer *mixer, uint8_t num_motors);

/** @brief Get calculated throttle for individual motor based on input for each axis.
 *
 * @param motor Motor to calculate throttle for.
 * @param in Controller output for each axis.
 *
 * @return Integer throttle value to be passed to the motor.
 */
uint32_t mixer_get_motor_throttle(enum motor motor, struct mixer *in);

/** @brief Configure deault Quad X mixer. */
void mixer_configure_default(void);

#endif  // __MIXER_H__
