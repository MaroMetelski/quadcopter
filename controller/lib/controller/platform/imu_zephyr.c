#include <controller/unit.h>
#include <zephyr/sys/__assert.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <controller/imu.h>

LOG_MODULE_REGISTER(imu_sensor, CONFIG_SENSOR_LOG_LEVEL);

BUILD_ASSERT(DT_NODE_EXISTS(DT_ALIAS(imu)), "No IMU device found under 'imu' alias!");
#define DT_IMU DT_ALIAS(imu)

static const struct device *imu = DEVICE_DT_GET(DT_IMU);

static void sensor_values_to_imu_data(struct sensor_value *accel_data,
        struct sensor_value *gyro_data, struct imu_data *imu_data) {
    for (int i = 0; i < 3; i++) {
        imu_data->accel[i] = (float)sensor_value_to_double(&accel_data[i]);
        imu_data->ang_v[i] = (float)sensor_value_to_double(&gyro_data[i]);
        imu_data->ang_v[i] = RAD_TO_DEG_F(imu_data->ang_v[i]);
    }
}

int imu_get_data(struct imu_data *imu_data) {
    __ASSERT(imu_data, "imu_data can't be NULL");
    struct sensor_value accel_data[3];
    struct sensor_value gyro_data[3];

    if (!device_is_ready(imu)) {
        LOG_ERR("IMU device not ready");
        return -ENODEV;
    }
    int err = sensor_sample_fetch(imu);
    if (err < 0) {
        LOG_ERR("Failed to fetch IMU sample");
        return -EIO;
    }
    err = sensor_channel_get(imu, SENSOR_CHAN_ACCEL_XYZ, accel_data);
    if (err < 0) {
        LOG_ERR("Failed to read accelerometer data");
        return -EIO;
    }
    err = sensor_channel_get(imu, SENSOR_CHAN_GYRO_XYZ, gyro_data);
    if (err < 0) {
        LOG_ERR("Failed to read gyroscope data");
        return -EIO;
    }
    sensor_values_to_imu_data(accel_data, gyro_data, imu_data);
    return 0;
}
