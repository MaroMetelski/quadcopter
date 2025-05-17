zephyr_library()

zephyr_library_sources(
    attitude.c
    calc.c
    configs.c
    config_dispatcher.c
    filter.c
    mixer.c
    pid_inst.c
    sbus_decoder.c
)

target_sources_ifdef(CONFIG_APP_MOTORS app PRIVATE motors.c)
target_sources_ifdef(CONFIG_APP_INPUT app PRIVATE input.c)

add_subdirectory(platform/)
