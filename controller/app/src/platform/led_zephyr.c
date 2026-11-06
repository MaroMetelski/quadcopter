#include <controller/low_level/led_ll.h>
#include <zephyr/device.h>
#include <zephyr/drivers/led.h>

const struct device *led = DEVICE_DT_GET(DT_LEDS);

void led_ll_on(uint8_t inst)
{
    (void)led_on(led, inst);
}

void led_ll_off(uint8_t inst)
{
    (void)led_off(led, inst);
}
