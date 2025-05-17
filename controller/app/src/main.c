#include "controller/controller.h"
#include "controller/pid.h"
#include "zephyr/sys/printk.h"
#include <controller/configs.h>
#include <controller/config_dispatcher.h>
#include <controller/logging.h>
#include <controller/imu.h>
#include <controller/filter.h>
#include <controller/attitude.h>
#include <controller/unit.h>
#include <controller/mixer.h>
#include <controller/pid_inst.h>
#include <controller/motors.h>
#include <controller/calc.h>

#include <zephyr/kernel.h>

APP_LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

struct {
    float throttle;
} input;

int main(void)
{
    if (!configs_load_all()) {
        LOG_ERR("Failed to load configuration from memory!");
        return -1;
    }
    LOG_INF("Loaded configuration from memory");

    if (!config_dispatcher_update_all()) {
        LOG_ERR("Failed to apply configurations!");
        return -1;
    }
    LOG_INF("Configuration applied to subsystems");

    // TODO: Implement separate controller module and use it to control mode
    struct controller_config ctrl_cfg;
    configs_get_controller(&ctrl_cfg);
    LOG_INF("Controller mode: %s", ctrl_cfg.mode == CONTROLLER_MODE_ANGLE ? "angle" : "rate");

    struct imu_data imu_data = {};
    float angles[2];
    float input_throttle = 0;
    mixer_configure_default();

    input_init();
    input_start();

    while (1) {
        /* Read IMU data.*/
        imu_get_data(&imu_data);
        /* Estimate attitude. Basic euler angles. */
        attitude_euler_from_acc(angles, imu_data.accel);

        /* Convert to degrees for further calculations. */
        for(int i = 0; i < 2; i++) {
            angles[i] = RAD_TO_DEG_F(angles[i]);
        }

        /* Read inputs. */
        float req_pitch = input_get_channel_value(CHANNEL_PITCH);
        float req_roll = input_get_channel_value(CHANNEL_ROLL);
        float req_yaw = input_get_channel_value(CHANNEL_YAW);

        /* Reduce sensitivity in the middle position. */
        req_pitch = IS_BETWEEN(req_pitch, -1, 1) ? 0 : req_pitch;
        req_roll = IS_BETWEEN(req_roll, -1, 1) ? 0 : req_roll;
        req_yaw = IS_BETWEEN(req_yaw, -1, 1) ? 0 : req_yaw;

        float req_pitch_rate = req_pitch;
        float req_roll_rate = req_roll;

        if (ctrl_cfg.mode == CONTROLLER_MODE_ANGLE) {
            float pitch_angle_error = req_pitch - angles[0];
            float roll_angle_error = req_roll - angles[1];
            req_pitch_rate = pid_inst_process(PID_ANGLE, AXIS_PITCH, pitch_angle_error);
            req_roll_rate = pid_inst_process(PID_ANGLE, AXIS_ROLL, roll_angle_error);
        }

        /* Angle PID controller provides target angular velocity. */
        float pitch_rate_error = req_pitch_rate - imu_data.ang_v[1];
        float roll_rate_error = req_roll_rate - imu_data.ang_v[0];
        float yaw_rate_error = req_yaw - RAD_TO_DEG_F(imu_data.ang_v[2]);

        /* Process rate PIDs. */
        float pitch_pid = pid_inst_process(PID_RATE, AXIS_PITCH, pitch_rate_error);
        float roll_pid = pid_inst_process(PID_RATE, AXIS_ROLL, roll_rate_error);
        float yaw_pid = pid_inst_process(PID_RATE, AXIS_YAW, yaw_rate_error);

        /* Get input. */
        input.throttle = input_get_channel_value(CHANNEL_THROTTLE);
        input_throttle = input.throttle;

        /* Failsafe. */
        if (input_throttle < 100) {
            motor_set(MOTOR_FRONT_LEFT, 0);
            motor_set(MOTOR_FRONT_RIGHT, 0);
            motor_set(MOTOR_BACK_LEFT, 0);
            motor_set(MOTOR_BACK_RIGHT, 0);
            pid_inst_reset_all();
            goto sleep;
        }

        struct mixer in = {
            .throttle = (int) input_throttle,
            .roll = (int) roll_pid,
            .yaw = (int) yaw_pid,
            .pitch = (int) pitch_pid,
        };

        /* Mix the output between motors. */
        uint32_t throttle_fl = mixer_get_motor_throttle(MOTOR_FRONT_LEFT, &in);
        uint32_t throttle_fr = mixer_get_motor_throttle(MOTOR_FRONT_RIGHT, &in);
        uint32_t throttle_bl = mixer_get_motor_throttle(MOTOR_BACK_LEFT, &in);
        uint32_t throttle_br = mixer_get_motor_throttle(MOTOR_BACK_RIGHT, &in);

        /* Apply throttle to motors. */
        /** TODO: Use percentage (perhaps float) to set throttle to motors.
            * This assumes knowledge about the configured range.
            */
        motor_set(MOTOR_FRONT_LEFT, throttle_fl > 1000 ? 1000 : throttle_fl);
        motor_set(MOTOR_FRONT_RIGHT, throttle_fr > 1000 ? 1000 : throttle_fr);
        motor_set(MOTOR_BACK_LEFT, throttle_bl > 1000 ? 1000 : throttle_bl);
        motor_set(MOTOR_BACK_RIGHT, throttle_br > 1000 ? 1000 : throttle_br);

sleep:
        k_sleep(K_MSEC(1));
    }
        return 0;
}
