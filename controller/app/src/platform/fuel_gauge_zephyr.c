#include "controller/fuel_gauge.h"
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(fuel_gauge, LOG_LEVEL_DBG);

BUILD_ASSERT(DT_NODE_EXISTS(DT_ALIAS(fuel_gauge)),
    "No fuel gauge device found under 'fuel-gauge' alias!");

#define DT_FUEL_GAUGE DT_ALIAS(fuel_gauge)

static const struct device *gauge = DEVICE_DT_GET(DT_FUEL_GAUGE);

static double fuel_gauge_get_sample(enum sensor_channel ch)
{
    if (!device_is_ready(gauge)) {
        LOG_ERR("Fuel gauge device not ready");
        return 0;
    }
    int err = sensor_sample_fetch(gauge);
    if (err < 0) {
        LOG_ERR("Failed to fetch fuel gauge sample");
        return 0;
    }

    struct sensor_value value;

    err = sensor_channel_get(gauge, ch, &value);

    if (err < 0) {
        LOG_ERR("Failed to read battery voltage!");
        return 0;
    }

    return sensor_value_to_double(&value);
}

double fuel_gauge_get_voltage(void)
{
    return fuel_gauge_get_sample(SENSOR_CHAN_GAUGE_VOLTAGE);
}

double fuel_gauge_get_charge(void)
{
    return fuel_gauge_get_sample(SENSOR_CHAN_GAUGE_STATE_OF_CHARGE);
}

double fuel_gauge_get_health(void)
{
    return fuel_gauge_get_sample(SENSOR_CHAN_GAUGE_STATE_OF_HEALTH);
}

double fuel_gauge_get_usable_capacity(void)
{
    return fuel_gauge_get_sample(SENSOR_CHAN_GAUGE_FULL_CHARGE_CAPACITY);
}

double fuel_gauge_get_current(void)
{
    return fuel_gauge_get_sample(SENSOR_CHAN_GAUGE_AVG_CURRENT);
}
