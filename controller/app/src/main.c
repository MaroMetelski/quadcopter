#include <controller/configs.h>
#include <controller/config_dispatcher.h>
#include <app/logging.h>
#include <zephyr/kernel.h>

APP_LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

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

    while (1) {
        k_sleep(K_MSEC(1000));
    }

    return 0;
}
