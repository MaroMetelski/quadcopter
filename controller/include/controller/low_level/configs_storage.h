#ifndef __CONFIGS_STORAGE_H__
#define __CONFIGS_STORAGE_H__

#include <stdbool.h>
#include <stdlib.h>
#include <controller/configs.h>

bool configs_storage_save_one(const char *key, const void *buffer, size_t len);
bool configs_storage_load_one(const char *key, const void *buffer, size_t len);

#endif  // __CONFIGS_STORAGE_H__
