target_sources(app PRIVATE
    imu_zephyr.c
    input_pwm_zephyr.c
    input_shell.c
    motors_pwm_zephyr.c
    motors_shell.c
    pid_cmsis.c
    quaternion_cmsis.c
)
