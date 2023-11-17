#include <controller/configs.h>
#include <controller/config_dispatcher.h>
#include <app/logging.h>
#include <controller/imu.h>
#include <controller/filter.h>
#include <controller/attitude.h>
#include <app/unit.h>
#include <controller/mixer.h>
#include <controller/pid_inst.h>
#include <controller/motors.h>
#include <app/calc.h>

#include <zephyr/kernel.h>

APP_LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define COMP_FILTER_ALPHA 0.9
#define MOTOR_SET_STEPS 1
#define OFFSET_SAMPLES_AMOUNT 200

struct {
    float throttle;
} input;

K_SEM_DEFINE(input_rdy, 0, 1);

void motor_set_gradual(enum motor motor, uint32_t target)
{
    uint32_t step = (uint32_t)(target / MOTOR_SET_STEPS);
    for (int i = 1; i <= MOTOR_SET_STEPS; i++) {
        motor_set(motor, i * step);
    }
}

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

    struct imu_data imu_data;
    float attitude_from_acc[3];
    float attitude_from_gyro[3];
    float attitude_filtered[3];
    float attitude_zero[3];
    float attitude_from_gyro_deg[3];

    float input_throttle = 0;
    mixer_configure_default();

    int64_t time_start = 0;
    int64_t time_end = 0;

    int offset_samples = 0;
    bool offsets_logged = false;

    input_init();
    input_start();

    while (1) {
        do {
            int64_t dt = time_end - time_start;

            time_start = k_uptime_get();
            /* Read IMU data.*/
            imu_get_data(&imu_data);

            /* Estimate attitude. */
            attitude_euler_from_acc(attitude_from_acc, imu_data.accel);
            attitude_propagate_euler(attitude_from_gyro, imu_data.ang_v, dt / 1000.0);

            /* Yaw can't be filtered, hence only 2 items. */
            filter_complimentary(attitude_from_gyro, attitude_from_acc, attitude_filtered, 2,
                COMP_FILTER_ALPHA);

            /* Update gyro measurements (without yaw) to provide new base for next loop. */
            memcpy(attitude_from_gyro, attitude_filtered, 2 * sizeof(float));

            /* Convert to degrees for further calculations. */
            for(int i = 0; i < 3; i++) {
                attitude_from_gyro_deg[i] = RAD_TO_DEG_F(attitude_from_gyro[i]);
            }
            /* 'rate' mode yaw. */
            attitude_from_gyro_deg[2] = RAD_TO_DEG_F(imu_data.ang_v[2]);

            /* Calculate the initial zero position. */
            if (offset_samples < OFFSET_SAMPLES_AMOUNT) {
                for (int i = 0; i < 3; i++) {
                    /** Cumulative average (Xn+1 + A * n) / (n + 1) */
                    attitude_zero[i] =
                        ((attitude_from_gyro_deg[i] + attitude_zero[i] * offset_samples)
                        / (offset_samples + 1));
                }
                offset_samples++;
                goto sleep;
            }
            if (!offsets_logged) {
                LOG_INF("Zero position: pitch=%.3f, roll=%.3f, yaw=%.3f",
                    attitude_zero[0], attitude_zero[1], attitude_zero[2]);
                offsets_logged = true;
            }

            /* Read inputs. */
            float pitch = input_get_channel_value(CHANNEL_PITCH);
            float roll = input_get_channel_value(CHANNEL_ROLL);
            float yaw = input_get_channel_value(CHANNEL_YAW);

            /* Reduce sensitivity in the middle position.
             *
             * TODO: This should be handled by input system.
             * A middle value could be calculated between min-max and a configuration
             * setting could be used to determine the "safe zone".
             */
            pitch = IS_BETWEEN(pitch, -1, 1) ? attitude_zero[0] : pitch;
            roll = IS_BETWEEN(roll, -1, 1) ? attitude_zero[1] : roll;
            yaw = IS_BETWEEN(yaw, -1, 1) ? attitude_zero[2] : yaw;

            /* Calculate errors. */
            float pitch_error = pitch - attitude_from_gyro_deg[0];
            float roll_error = roll - attitude_from_gyro_deg[1];
            float yaw_error = yaw - attitude_from_gyro_deg[2];

            /* Process PIDs. */
            float pitch_pid = pid_inst_process(AXIS_PITCH, pitch_error);
            float roll_pid = pid_inst_process(AXIS_ROLL, roll_error);
            float yaw_pid = pid_inst_process(AXIS_YAW, yaw_error);

            /* Cap the output of PID controller.
             *
             * TODO: This should be based on the throttle range.
             */
            pitch_pid = pitch_pid >= 300 ? 300 : pitch_pid;
            roll_pid = roll_pid >= 300 ? 300 : roll_pid;
            yaw_pid = yaw_pid >= 300 ? 300 : yaw_pid;

            /* Get input. */
            input.throttle = input_get_channel_value(CHANNEL_THROTTLE);
            input_throttle = input.throttle;

            /* Failsafe. */
            if (input_throttle < 100) {
                motor_set(MOTOR_FRONT_LEFT, 0);
                motor_set(MOTOR_FRONT_RIGHT, 0);
                motor_set(MOTOR_BACK_LEFT, 0);
                motor_set(MOTOR_BACK_RIGHT, 0);
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
            /* TODO: Use percentage (perhaps float) to set throttle to motors.
             * This assumes knowledge about the configured range.
             */
            motor_set_gradual(MOTOR_FRONT_LEFT, throttle_fl > 1000 ? 1000 : throttle_fl);
            motor_set_gradual(MOTOR_FRONT_RIGHT, throttle_fr > 1000 ? 1000 : throttle_fr);
            motor_set_gradual(MOTOR_BACK_LEFT, throttle_bl > 1000 ? 1000 : throttle_bl);
            motor_set_gradual(MOTOR_BACK_RIGHT, throttle_br > 1000 ? 1000 : throttle_br);

sleep:
            k_sleep(K_MSEC(1));
            time_end = k_uptime_get();
        } while (0);
    }
        return 0;
}
