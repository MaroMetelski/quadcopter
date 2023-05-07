#ifndef __LOG_H__
#define __LOG_H__

#ifdef TEST

#define APP_LOG_MODULE_REGISTER(...)
#define APP_LOG_ERR(...)
#define APP_LOG_WRN(...)
#define APP_LOG_INF(...)
#define APP_LOG_DBG(...)
#define APP_LOG_LEVEL_DBG 0
#define APP_LOG_LEVEL_INF 0
#define APP_LOG_LEVEL_WRN 0
#define APP_LOG_LEVEL_ERR 0

#else

#include <zephyr/logging/log.h>

#define APP_LOG_MODULE_REGISTER(...) LOG_MODULE_REGISTER(__VA_ARGS__)
#define APP_LOG_ERR(...) LOG_ERR(__VA_ARGS__)
#define APP_LOG_WRN(...) LOG_WRN(__VA_ARGS__)
#define APP_LOG_INF(...) LOG_INF(__VA_ARGS__)
#define APP_LOG_DBG(...) LOG_DBG(__VA_ARGS__)
#define APP_LOG_LEVEL_DBG LOG_LEVEL_DBG
#define APP_LOG_LEVEL_INF LOG_LEVEL_INF
#define APP_LOG_LEVEL_WRN LOG_LEVEL_WRN
#define APP_LOG_LEVEL_ERR LOG_LEVEL_ERR

#endif  // TEST

#endif  // __LOG_H__
