#include <stdlib.h>
#include <zephyr/shell/shell.h>
#include <controller/configs.h>
#include <controller/config_dispatcher.h>

static int cmd_config_set_pid(const struct shell *sh, size_t argc, char **argv)
{
    enum pid_config_key key;

    if (0 == strcmp(argv[1], "pitch")) {
        key = CONFIG_PID_PITCH;
    } else if (0 == strcmp(argv[1], "roll")) {
        key = CONFIG_PID_ROLL;
    } else if (0 == strcmp(argv[1], "yaw")) {
        key = CONFIG_PID_YAW;
    } else {
        shell_error(sh, "Invalid PID instance! %s", argv[1]);
        return -1;
    }

    float Kp = atof(argv[2]);
    float Ki = atof(argv[3]);
    float Kd = atof(argv[4]);

    struct pid_config cfg = {
        .Kd = Kd,
        .Ki = Ki,
        .Kp = Kp,
    };

    if (!configs_set_pid(key, &cfg)) {
        shell_error(sh, "Failed to set PID config!");
        return -1;
    }

    return 0;
}

static int cmd_config_get_pid(const struct shell *sh, size_t argc, char **argv)
{
    enum pid_config_key key;

    if (0 == strcmp(argv[1], "pitch")) {
        key = CONFIG_PID_PITCH;
    } else if (0 == strcmp(argv[1], "roll")) {
        key = CONFIG_PID_ROLL;
    } else if (0 == strcmp(argv[1], "yaw")) {
        key = CONFIG_PID_YAW;
    } else {
        shell_error(sh, "Invalid PID instance! %s", argv[1]);
        return -1;
    }

    struct pid_config cfg;

    if (!configs_get_pid(key, &cfg)) {
        shell_error(sh, "Failed to read PID config!");
        return -1;
    }

    shell_print(sh, "PID %s: Kp=%.3f, Kd=%.3f, Ki=%.3f", argv[1], cfg.Kp, cfg.Kd, cfg.Ki);

    return 0;
}

static int cmd_config_apply_all(const struct shell *sh, size_t argc, char **argv)
{
    if (!config_dispatcher_update_all()) {
        shell_error(sh, "Failed to dispatch configurations!");
        return -1;
    }

    return 0;
}

static int cmd_config_save_all(const struct shell *sh, size_t argc, char **argv)
{
    if (!configs_save_all()) {
        shell_error(sh, "Failed to save configurations!");
        return -1;
    }
    return 0;
}

static int cmd_config_load_all(const struct shell *sh, size_t argc, char **argv)
{
    if (!configs_load_all()) {
        shell_error(sh, "Failed to load configurations!");
        return -1;
    }
    return 0;
}

static int cmd_config_set_input(const struct shell *sh, size_t argc, char **argv)
{
    enum input_config_key key;

    if (0 == strcmp(argv[1], "pitch")) {
        key = CONFIG_INPUT_CHANNEL_PITCH_CONFIG;
    } else if (0 == strcmp(argv[1], "roll")) {
        key = CONFIG_INPUT_CHANNEL_ROLL_CONFIG;
    } else if (0 == strcmp(argv[1], "yaw")) {
        key = CONFIG_INPUT_CHANNEL_YAW_CONFIG;
    } else if (0 == strcmp(argv[1], "throttle")) {
        key = CONFIG_INPUT_CHANNEL_THROTTLE_CONFIG;
    } else {
        shell_error(sh, "Invalid input channel! %s", argv[1]);
        return -1;
    }

    struct input_channel_config cfg = {
        .channel = atoi(argv[2]),
        .min_val = atof(argv[3]),
        .max_val = atof(argv[4]),
    };

    if (!configs_set_input_config(key, &cfg)) {
        shell_error(sh, "Failed to set input channel config!");
        return -1;
    }
    return 0;
}

static int cmd_config_get_input(const struct shell *sh, size_t argc, char **argv)
{
    enum input_config_key key;

    if (0 == strcmp(argv[1], "pitch")) {
        key = CONFIG_INPUT_CHANNEL_PITCH_CONFIG;
    } else if (0 == strcmp(argv[1], "roll")) {
        key = CONFIG_INPUT_CHANNEL_ROLL_CONFIG;
    } else if (0 == strcmp(argv[1], "yaw")) {
        key = CONFIG_INPUT_CHANNEL_YAW_CONFIG;
    } else if (0 == strcmp(argv[1], "throttle")) {
        key = CONFIG_INPUT_CHANNEL_THROTTLE_CONFIG;
    } else {
        shell_error(sh, "Invalid input channel! %s", argv[1]);
        return -1;
    }

    struct input_channel_config cfg;

    if (!configs_get_input_config(key, &cfg)) {
        shell_error(sh, "Failed to get input channel config!");
        return -1;
    }

    shell_print(sh, "%s input cfg: pwm_ch=%d, min=%.3f, max=%.3f",
        argv[1], cfg.channel, cfg.min_val, cfg.max_val);

    return 0;
}

static int cmd_config_get_input_calib(const struct shell *sh, size_t argc, char **argv)
{
    enum input_config_key key;

    if (0 == strcmp(argv[1], "pitch")) {
        key = CONFIG_INPUT_CHANNEL_PITCH_CONFIG;
    } else if (0 == strcmp(argv[1], "roll")) {
        key = CONFIG_INPUT_CHANNEL_ROLL_CONFIG;
    } else if (0 == strcmp(argv[1], "yaw")) {
        key = CONFIG_INPUT_CHANNEL_YAW_CONFIG;
    } else if (0 == strcmp(argv[1], "throttle")) {
        key = CONFIG_INPUT_CHANNEL_THROTTLE_CONFIG;
    } else {
        shell_error(sh, "Invalid input channel! %s", argv[1]);
        return -1;
    }

    struct input_channel_pwm_calib calib;

    if (!configs_get_input_calib(key, &calib)) {
        shell_error(sh, "Failed to get input channel calibration!");
        return -1;
    }

    shell_print(sh, "%s input calib: min_dc=%d, max_dc=%d", argv[1], calib.min_dc, calib.max_dc);

    return 0;
}

static int cmd_config_set_input_calib(const struct shell *sh, size_t argc, char **argv)
{
    enum input_config_key key;

    if (0 == strcmp(argv[1], "pitch")) {
        key = CONFIG_INPUT_CHANNEL_PITCH_CONFIG;
    } else if (0 == strcmp(argv[1], "roll")) {
        key = CONFIG_INPUT_CHANNEL_ROLL_CONFIG;
    } else if (0 == strcmp(argv[1], "yaw")) {
        key = CONFIG_INPUT_CHANNEL_YAW_CONFIG;
    } else if (0 == strcmp(argv[1], "throttle")) {
        key = CONFIG_INPUT_CHANNEL_THROTTLE_CONFIG;
    } else {
        shell_error(sh, "Invalid input channel! %s", argv[1]);
        return -1;
    }

    struct input_channel_pwm_calib calib = {
        .min_dc = atoi(argv[2]),
        .max_dc = atoi(argv[3]),
    };

    if (!configs_set_input_calib(key, &calib)) {
        shell_error(sh, "Failed to set input channel config!");
        return -1;
    }
    return 0;
}

static int cmd_config_motor_set(const struct shell *sh, size_t argc, char **argv)
{
    enum motor_config_key key;
    if (0 == strcmp(argv[1], "fl")) {
        key = CONFIG_MOTOR_FL_CONFIG;
    } else if (0 == strcmp(argv[1], "bl")) {
        key = CONFIG_MOTOR_BL_CONFIG;
    } else if (0 == strcmp(argv[1], "fr")) {
        key = CONFIG_MOTOR_FR_CONFIG;
    } else if (0 == strcmp(argv[1], "br")) {
        key = CONFIG_MOTOR_BR_CONFIG;
    } else {
        shell_error(sh, "Invalid motor! %s", argv[1]);
        return -1;
    }

    struct motor_config cfg = {
        .channel = atoi(argv[2]),
        .max_throttle = atoi(argv[3]),
    };

    if (!configs_set_motor(key, &cfg)) {
        shell_error(sh, "Failed to set motor config!");
        return -1;
    }

    return 0;
}

static int cmd_config_motor_get(const struct shell *sh, size_t argc, char **argv)
{
    enum motor_config_key key;
    if (0 == strcmp(argv[1], "fl")) {
        key = CONFIG_MOTOR_FL_CONFIG;
    } else if (0 == strcmp(argv[1], "bl")) {
        key = CONFIG_MOTOR_BL_CONFIG;
    } else if (0 == strcmp(argv[1], "fr")) {
        key = CONFIG_MOTOR_FR_CONFIG;
    } else if (0 == strcmp(argv[1], "br")) {
        key = CONFIG_MOTOR_BR_CONFIG;
    } else {
        shell_error(sh, "Invalid motor! %s", argv[1]);
        return -1;
    }

    struct motor_config cfg;
    if (!configs_get_motor(key, &cfg)) {
        shell_error(sh, "Failed to get motor config!");
        return -1;
    }

    shell_print(sh, "%s motor cfg: ch=%d, max_throttle=%d",
        argv[1], cfg.channel, cfg.max_throttle);

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(config_pid_sub_cmds,
    SHELL_CMD_ARG(set, NULL, "set <inst> <Kp> <Ki> <Kd>", cmd_config_set_pid, 5, 0),
    SHELL_CMD_ARG(get, NULL, "get <inst>", cmd_config_get_pid, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(config_input_sub_cmds,
    SHELL_CMD_ARG(set, NULL, "set <channel> <pwm_ch> <min> <max>", cmd_config_set_input, 5, 0),
    SHELL_CMD_ARG(get, NULL, "get <channel>", cmd_config_get_input, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(config_input_calib_sub_cmds,
    SHELL_CMD_ARG(set, NULL, "set <channel> <min> <max>", cmd_config_set_input_calib, 4, 0),
    SHELL_CMD_ARG(get, NULL, "get <channel>", cmd_config_get_input_calib, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(config_motor_sub_cmds,
    SHELL_CMD_ARG(set, NULL, "set <motor> <channel> <max>", cmd_config_motor_set, 4, 0),
    SHELL_CMD_ARG(get, NULL, "get <motor>", cmd_config_motor_get, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(config_sub_cmds,
    SHELL_CMD(input_calib, &config_input_calib_sub_cmds, "input_calib <get|set>", NULL),
    SHELL_CMD(input_config, &config_input_sub_cmds, "input_config <get|set>", NULL),
    SHELL_CMD(motor, &config_motor_sub_cmds, "motor <get|set>", NULL),
    SHELL_CMD(pid, &config_pid_sub_cmds, "pid <get|set>", NULL),
    SHELL_CMD(apply_all, NULL, "Apply all configurations", cmd_config_apply_all),
    SHELL_CMD(save_all, NULL, "Save configurations to memory", cmd_config_save_all),
    SHELL_CMD(load_all, NULL, "Load configurations from memory", cmd_config_load_all),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(config, &config_sub_cmds, "config module commands", NULL);
