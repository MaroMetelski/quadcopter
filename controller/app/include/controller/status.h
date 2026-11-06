#ifndef STATUS_H_
#define STATUS_H_

enum status {
    BATTERY_CHARGE = 0, // status of the battery charge
    MOTORS_ARMING, // status of the motor arming
    STATUS_LAST, // enum delimeter
};

enum battery_charge_status {
    BATTERY_CHARGE_UNKNOWN = 0,
    BATTERY_CHARGE_OK,
    BATTERY_CHARGE_WARNING,
    BATTERY_CHARGE_CRITICAL,
    BATTERY_CHARGE_LAST,
};

enum motors_arm_status {
    MOTORS_DISARMED = 0,
    MOTORS_ARMED,
    MOTORS_ARM_LAST,
};

enum event {
    EVENT_MOTORS_ARMED = 1,
    EVENT_MOTORS_DISARMED,
    EVENT_CHARGER_CONNECTED,
    EVENT_CHARGER_DISCONNECTED,
};

#endif /* STATUS_H_ */
