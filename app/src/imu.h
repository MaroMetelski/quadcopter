#ifndef __SRC_IMU_H__
#define __SRC_IMU_H__

struct imu_data {
    float ang_v[3];
    float accel[3];
};

int imu_get_data(struct imu_data *data);

#endif  // __SRC_IMU_H__
