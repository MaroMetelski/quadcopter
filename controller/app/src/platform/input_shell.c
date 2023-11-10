#include <stdlib.h>
#include <zephyr/shell/shell.h>
#include <controller/input.h>

static int cmd_input_configure(
    const struct shell *sh, size_t argc, char **argv)
{
    char *channel_string = argv[1];
    enum input_channel channel = input_channel_from_string(channel_string);
    if (channel >= CHANNEL_FUNC_LAST) {
        shell_error(sh, "Invalid channel '%s'", channel_string);
        return -1;
    }
    struct input_channel_config cfg = {
        .channel = atoi(argv[2]),
        .min_val = atof(argv[3]),
        .max_val = atof(argv[4]),
    };
    if (!input_configure_channel(channel, &cfg)) {
        shell_error(sh, "Failed to configure channel '%s'", channel_string);
        return -1;
    }
    shell_info(sh, "Configured channel '%s': min_val=%f, max_val=%f, pwm_channel=%d",
        channel_string, cfg.min_val, cfg.max_val, cfg.channel);
    return 0;
}

static int cmd_input_read_raw(const struct shell *sh, size_t argc, char **argv)
{
    char *channel_string = argv[1];
    enum input_channel channel = input_channel_from_string(channel_string);
    if (channel >= CHANNEL_FUNC_LAST) {
        shell_error(sh, "Invalid channel '%s'", channel_string);
        return -1;
    }
    uint32_t ch_val = input_get_channel_raw_value(channel);
    shell_print(sh, "channel '%s' value=%d", channel_string, ch_val);
    return 0;
}

static int cmd_input_read(const struct shell *sh, size_t argc, char **argv)
{
    char *channel_string = argv[1];
    enum input_channel channel = input_channel_from_string(channel_string);
    if (channel >= CHANNEL_FUNC_LAST) {
        shell_error(sh, "Invalid channel '%s'", channel_string);
        return -1;
    }
    float ch_val = input_get_channel_value(channel);
    shell_print(sh, "channel '%s' value=%.3f", channel_string, ch_val);
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(input_sub_cmds,
    SHELL_CMD_ARG(configure, NULL, "configure <channel> <pwm_ch> <value_min> <value_max>", cmd_input_configure, 5, 0),
    SHELL_CMD_ARG(read, NULL, "read <channel>", cmd_input_read, 2, 0),
    SHELL_CMD_ARG(read_raw, NULL, "read_raw <channel>", cmd_input_read_raw, 2, 0),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(input, &input_sub_cmds, "input module commands", NULL);
