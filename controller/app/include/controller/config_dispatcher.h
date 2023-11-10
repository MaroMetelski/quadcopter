#ifndef __CONFIG_DISPATCHER_H__
#define __CONFIG_DISPATCHER_H__

#include <stdbool.h>

bool config_dispatcher_update_motors(void);
bool config_dispatcher_update_pid(void);
bool config_dispatcher_update_input(void);
bool config_dispatcher_update_all(void);

#endif  // __CONFIG_DISPATCHER_H__
