#include <cmsis_os2.h>
#include <controller/low_level/led_ll.h>
#include <controller/status.h>
#include <controller/indications.h>
#include <app/logging.h>

APP_LOG_MODULE_REGISTER(indications, APP_LOG_LEVEL_DBG);

#define MSQ_OBJECTS 10

struct indication_msg {
    struct indication_config *buf;
};

static struct {
    int idx;
    osThreadId_t tid;
    osThreadAttr_t tattr;
    osMessageQueueId_t queue;
} leds[NUM_LEDS];

static struct indication_config battery_charge_ind[BATTERY_CHARGE_LAST] = {
    [BATTERY_CHARGE_UNKNOWN] = { // battery ok, blue LED off
        .resource = INDICATION_LED,
        .led = {
            .idx = 0,
            .type = INDICATION_LED_STATIC,
            .led_static_on = false,
        },
    },
    [BATTERY_CHARGE_WARNING] = { // battery warning, blue led static
        .resource = INDICATION_LED,
        .led = {
            .idx = 0,
            .type = INDICATION_LED_STATIC,
            .led_static_on = true,
        },
    },
    [BATTERY_CHARGE_CRITICAL] = { // battery critical, blue led blinking
        .resource = INDICATION_LED,
        .led = {
            .idx = 0,
            .type = INDICATION_LED_BLINK,
            .led_blink = {
                .time_off = 500,
                .time_on = 500,
            },
        },
    },
};

static struct indication_config motor_armed_ind[MOTORS_ARM_LAST] = {
    [MOTORS_ARMED] = {
        .resource = INDICATION_LED,
        .led = {
            .idx = 1,
            .type = INDICATION_LED_STATIC,
            .led_static_on = true,
        },
    },
};

/* Entry function for a LED thread. Pointer to LED idx should be passed as argument. */
static void led_thread_entry(void *arg);
/* Wait for next indication for the specified LED. */
static struct indication_config *wait_next_led_indication(uint8_t led, int timeout);
/* Publish the specified indication to the corresponding resource. */
static void publish_indication(struct indication_config *cfg);

/* Blink an LED with the specified time off and time on and wait for next indications.
 * Will stop and return upon reception of a new indication.
 */
static struct indication_config *blink(uint8_t led, int t_on, int t_off);
/* Similar to 'blink', but simply turn the LED on or off. */
static struct indication_config *led_static(uint8_t led, bool on);


static struct indication_config *wait_next_led_indication(uint8_t led, int timeout)
{
    struct indication_msg msg;
    if (osOK == osMessageQueueGet(leds[led].queue, &msg, NULL, timeout))
    {
        struct indication_config *cfg = (struct indication_config *)msg.buf;
        return cfg;
    }
    return NULL;
}

static void publish_indication(struct indication_config *cfg)
{
    enum indication_resource rsc = cfg->resource;
    if (rsc == INDICATION_LED) {
        int led_idx = cfg->led.idx;
        struct indication_msg msg;
        msg.buf = cfg;
        osMessageQueuePut(leds[led_idx].queue, &msg, 0, 0);
    }
}

static struct indication_config *blink(uint8_t led, int t_on, int t_off)
{
    struct indication_config *next = NULL;
    while(true) {
        led_ll_on(led);
        next = wait_next_led_indication(led, t_on);
        if (next) {
            return next;
        }
        led_ll_off(led);
        next = wait_next_led_indication(led, t_off);
        if (next) {
            return next;
        }
    }
}

static struct indication_config *led_static(uint8_t led, bool on)
{
    struct indication_config *next = NULL;
    if (on) {
        led_ll_on(led);
    } else {
        led_ll_off(led);
    }
    next = wait_next_led_indication(led, osWaitForever);
    return next;
}

static void led_thread_entry(void *arg)
{
    int led_inst = *(uint8_t*)arg;
    struct indication_config *next = wait_next_led_indication(led_inst, osWaitForever);;
    // ASSERT(next); // Should never be NULL
    while (true) {
        switch (next->led.type) {
        case INDICATION_LED_STATIC: {
            bool on = next->led.led_static_on;
            next = led_static(led_inst, on);
            break;
        }
        case INDICATION_LED_BLINK: {
            int t_off = next->led.led_blink.time_off;
            int t_on = next->led.led_blink.time_on;
            next = blink(led_inst, t_off, t_on);
            break;
        }
        default:
            break;
        }
        // ASSERT(next); // Should never be NULL
    }
}

void indications_notify_status(enum status status, int flag)
{
    switch (status) {
    case BATTERY_CHARGE: {
        struct indication_config *cfg = &battery_charge_ind[flag];
        publish_indication(cfg);
        break;
    }
    case MOTORS_ARMING: {
        struct indication_config *cfg = &motor_armed_ind[flag];
        publish_indication(cfg);
        break;
    }
    default:
        break;
    }
}

void indications_init(void)
{
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].idx = i;
        leds[i].tattr.priority = osPriorityLow;
        leds[i].tid = osThreadNew(led_thread_entry, &leds[i].idx, &leds[i].tattr);
        leds[i].queue = osMessageQueueNew(MSQ_OBJECTS, sizeof(struct indication_msg), NULL);
        // ASSERT(leds[i].tid && leds[i].queue);
        LOG_INF("Created indication thread for LED: %d", leds[i].idx);
    }
}
