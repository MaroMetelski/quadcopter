target_sources(app PRIVATE
    attitude.c
    calc.c
    filter.c
    imu_zephyr.c
    input.c
    input_pwm_zephyr.c
    input_shell.c
    main.c
    mixer.c
    motors_pwm_zephyr.c
    motors_shell.c
    motors.c
    pid_cmsis.c
    pid_inst.c
    quaternion_cmsis.c
)
