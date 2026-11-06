#ifndef INDICATIONS_H_
#define INDICATIONS_H_

#include <stdint.h>
#include <controller/status.h>
#include <stdbool.h>

/* Default priority for the system indications. */
#define IND_SYSTEM_DEFAULT_PRIO 0
/* Default priority for the battery indications. */
#define IND_BATTERY_DEFAULT_PRIO 1
/* Default priority for motor indications. */
#define IND_MOTORS_DEFAULT_PRIO 2

enum indication_resource {
    INDICATION_LED,
    INDICATION_BUZZER,
    INDICATION_TYPE_LAST,
};

enum indication_led_type {
    INDICATION_LED_NONE = 0,
    INDICATION_LED_STATIC,
    INDICATION_LED_BLINK,
    INDICATION_LED_LAST,
};

struct indication_led_blink {
    int time_on;
    int time_off;
};

struct indication_led {
    int idx;
    enum indication_led_type type;
    union {
        struct indication_led_blink led_blink;
        bool led_static_on;
    };
};

/* Configuration for an indication module. */
struct indication_config {
    enum indication_resource resource; // Type of resource to be used, e.g. LED
    union {
        const struct indication_led led; // array of LED indication patterns
    };
    int event_duration; // indication duration, used only for events (not status)
};

void indications_init(void);
void indications_notify_status(enum status status, int flag);

#endif
