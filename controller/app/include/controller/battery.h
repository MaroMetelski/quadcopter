#ifndef BATTERY_H_
#define BATTERY_H_

#define BATTERY_CHARGE_WARNING_THRESHOLD 70
#define BATTERY_CHARGE_CRITICAL_THRESHOLD 30
#define BATTERY_MONITOR_INTERVAL 1000

void battery_monitor_start(void);

#endif /* BATTERY_H_ */
