#ifndef LED_H_
#define LED_H_

#include <stdint.h>
#include <stdbool.h>

/* TODO: Don't include for test, this is purely hardware configuration based on platform. */
#include <controller/low_level/led_cfg.h>

#if (!defined(NUM_LEDS) || NUM_LEDS == 0)
#warning "No LEDs present in hardware configuration"
#endif

void led_ll_on(uint8_t inst);
void led_ll_off(uint8_t inst);

#endif /* LED_H_ */
