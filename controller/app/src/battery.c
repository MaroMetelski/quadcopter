#include <cmsis_os2.h>
#include <controller/fuel_gauge.h>
#include <controller/battery.h>
#include <controller/indications.h>

static osThreadId_t tid;
static osThreadAttr_t tattr;

static void battery_monitor_thread(void *arg)
{
    while (true) {
        double charge = fuel_gauge_get_charge();
        if (charge <= BATTERY_CHARGE_WARNING_THRESHOLD
                && charge > BATTERY_CHARGE_CRITICAL_THRESHOLD) {
            indications_notify_status(BATTERY_CHARGE, BATTERY_CHARGE_WARNING);
        } else if (charge <= BATTERY_CHARGE_CRITICAL_THRESHOLD) {
            indications_notify_status(BATTERY_CHARGE, BATTERY_CHARGE_CRITICAL);
        }
        osDelay(BATTERY_MONITOR_INTERVAL);
    }
}

void battery_monitor_start(void)
{
    tattr.priority = osPriorityLow;
    tid = osThreadNew(battery_monitor_thread, NULL, &tattr);
}
