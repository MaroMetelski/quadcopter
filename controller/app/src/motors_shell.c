#include <stdlib.h>
#include <zephyr/shell/shell.h>
#include <app/motors.h>

static int cmd_motor_configure(
    const struct shell *sh, size_t argc, char **argv)
{
    char *motor_string = argv[1];
    enum motor motor = motor_from_string(motor_string);
    if (motor >= MOTOR_LAST) {
        shell_error(sh, "Invalid channel '%s'", motor_string);
        return -1;
    }
    struct motor_config cfg = {
        .channel = atoi(argv[2]),
        .max_throttle = atoi(argv[3]),
    };
    if (!motor_configure(motor, &cfg)) {
        shell_error(sh, "Failed to configure motor '%s'", motor_string);
        return -1;
    }
    shell_info(sh, "Configured motor '%s': max_throttle=%d, pwm_channel=%d",
        motor_string, cfg.max_throttle, cfg.channel);
    return 0;
}

static int cmd_motor_set(const struct shell *sh, size_t argc, char **argv)
{
    char *motor_string = argv[1];
    enum motor motor = motor_from_string(motor_string);
    if (motor >= MOTOR_LAST) {
        shell_error(sh, "Invalid motor '%s'", motor_string);
        return -1;
    }
    uint32_t throttle = atoi(argv[2]);
    motor_set(motor, throttle);
    shell_print(sh, "motor '%s' throttle=%d", motor_string, throttle);
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(motors_sub_cmds,
    SHELL_CMD_ARG(configure, NULL, "configure <motor> <pwm_ch> <max_throttle>", cmd_motor_configure, 4, 0),
    SHELL_CMD_ARG(set, NULL, "set <motor> <throttle>", cmd_motor_set, 3, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(motors, &motors_sub_cmds, "motors module commands", NULL);
