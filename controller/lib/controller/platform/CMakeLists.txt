zephyr_library()

zephyr_library_sources(
    configs_storage_zephyr.c
    pid_cmsis.c
    config_shell.c
)

zephyr_library_sources_ifdef(CONFIG_APP_IMU_ZEPHYR imu_zephyr.c)
zephyr_library_sources_ifdef(CONFIG_APP_MOTORS_PWM_ZEPHYR
    motors_pwm_zephyr.c
    motors_shell.c)

zephyr_library_sources_ifdef(CONFIG_APP_INPUT input_shell.c)

zephyr_library_sources_ifdef(CONFIG_APP_INPUT_SBUS_ZEPHYR input_sbus_zephyr.c)
