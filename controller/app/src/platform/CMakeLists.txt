target_sources(app PRIVATE
    configs_storage_zephyr.c
    pid_cmsis.c
    quaternion_cmsis.c
    config_shell.c
)

target_sources_ifdef(CONFIG_APP_IMU_ZEPHYR app PRIVATE imu_zephyr.c)
target_sources_ifdef(CONFIG_APP_MOTORS_PWM_ZEPHYR app PRIVATE
    motors_pwm_zephyr.c
    motors_shell.c)

target_sources_ifdef(CONFIG_APP_INPUT_PWM_ZEPHYR app PRIVATE
    input_pwm_zephyr.c
    input_shell.c)
