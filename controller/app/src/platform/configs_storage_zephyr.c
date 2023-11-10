#include <errno.h>
#include <zephyr/settings/settings.h>
#include <zephyr/init.h>
#include <controller/low_level/configs_storage.h>

struct direct_load_data {
    size_t size;
    void *dest;
};

static int direct_load_data(const char *name, size_t len,
    settings_read_cb read_cb, void *cb_arg, void *param)
{
    const char *next;
    size_t name_len;
    int rc;

    struct direct_load_data *value = (struct direct_load_data *)param;

    name_len = settings_name_next(name, &next);

    if (name_len == 0) {
        if (len == value->size) {
            rc = read_cb(cb_arg, value->dest, len);
            if (rc < 0) {
                return rc;
            }
        }
        return -EINVAL;
    }
    return 0;
}

bool configs_storage_load_one(const char *key, const void *buffer, size_t len)
{
    int rc;
    struct direct_load_data data = {
        .dest = buffer,
        .size = len,
    };

    rc = settings_load_subtree_direct(key, direct_load_data, (void *)&data);
    if (rc != 0) {
        return false;
    }
    return true;
}

bool configs_storage_save_one(const char *key, const void *buffer, size_t len)
{
    if (settings_save_one(key, buffer, len) != 0) {
        return false;
    }
    return true;
}

SYS_INIT(settings_subsys_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
