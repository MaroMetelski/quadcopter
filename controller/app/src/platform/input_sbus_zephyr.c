#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>
#include <controller/low_level/input_sbus.h>
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(sbus_uart, CONFIG_APP_INPUT_SBUS_LOG_LEVEL);

BUILD_ASSERT(DT_NODE_EXISTS(DT_ALIAS(sbus)), "No sbus device found!");
#define SBUS_UART_NODE   DT_ALIAS(sbus)

/* SBus frame size with header and footer. */
#define SBUS_FRAME_SIZE 25
/* Amount of SBus bytes to read after header has been captured. */
#define SBUS_BYTES_TO_READ (SBUS_FRAME_SIZE - 1)

#define SBUS_HEADER     0x0F
#define SBUS_FOOTER     0x00

static const struct device *sbus = DEVICE_DT_GET(SBUS_UART_NODE);

static enum {
    SBUS_WAITING,
    SBUS_RX,
} current_state;

static struct {
    uint8_t buf[SBUS_FRAME_SIZE];
    uint8_t size;
} rx;

static input_ll_frame_callback frame_cb;

void sbus_uart_cb(const struct device *dev, void *user_data) {
    if (!uart_irq_update(sbus)) {
        return;
    }

    if (!uart_irq_rx_ready(sbus)) {
        return;
    }
    uint8_t c;
    /**Capture frame header. */
    while (current_state == SBUS_WAITING && uart_fifo_read(sbus, &c, 1) != 0) {
        if (c == SBUS_HEADER) {
            current_state = SBUS_RX;
            rx.size = 0;
            memset(rx.buf, 0, SBUS_FRAME_SIZE);
        }
    }
    /* Remaining amount of bytes + footer. */
    int to_read = SBUS_BYTES_TO_READ;
    int read = 0;
    /* Read 25 characters. */
    while (current_state == SBUS_RX) {
        read = uart_fifo_read(sbus, &rx.buf[rx.size], to_read);
        rx.size += read;
        if (read == 0) {
            break;
        }
    }

    if (rx.size == SBUS_BYTES_TO_READ) {
        current_state = SBUS_WAITING;
        if (rx.buf[rx.size] != 0x00) {
            LOG_ERR("Invalid footer, discarding frame!");
            return;
        }
        /* Validate only the actual data bytes. */
        rx.size--;
        LOG_HEXDUMP_DBG(rx.buf, rx.size, "sbus data");
        if (frame_cb) {
            frame_cb(rx.buf, rx.size);
        }
    }
}

int input_sbus_set_frame_callback(input_ll_frame_callback cb)
{
    if (!cb) {
        return -EINVAL;
    }
    frame_cb = cb;
    return 0;
}

int input_sbus_start(void)
{
    if (!device_is_ready(sbus)) {
        return -ENODEV;
    }
    int ret = uart_irq_callback_user_data_set(sbus, sbus_uart_cb, NULL);
    if (ret < 0) {
        LOG_ERR("Failed to configure SBus IRQ UART!");
        return ret;
    }
    uart_irq_rx_enable(sbus);
    return 0;
}

int input_sbus_stop(void)
{
    uart_irq_rx_disable(sbus);
    return 0;
}
