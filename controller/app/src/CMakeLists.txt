target_sources(app PRIVATE
    attitude.c
    calc.c
    filter.c
    main.c
    mixer.c
    pid_inst.c
)

target_sources_ifdef(CONFIG_APP_MOTORS app PRIVATE motors.c)
target_sources_ifdef(CONFIG_APP_INPUT app PRIVATE input.c)
