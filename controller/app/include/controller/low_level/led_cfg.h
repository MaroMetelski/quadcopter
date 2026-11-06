#ifndef LED_CFG_H_
#define LED_CFG_H_

#include <zephyr/devicetree.h>

BUILD_ASSERT(DT_NODE_EXISTS(DT_ALIAS(leds)), "No LEDs node found under 'leds' alias!");

#define DT_LEDS DT_ALIAS(leds)

#define NUM_LEDS DT_CHILD_NUM(DT_LEDS)

#endif
