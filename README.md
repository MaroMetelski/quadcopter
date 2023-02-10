# RP2040 quadcopter controller software

Quadcopter controller software for RP2040 based hardware.

## Development

### RaspberryPi Pico

#### Requirements

- OpenOCD - RaspberryPi fork ([link](https://github.com/raspberrypi/openocd)),
- [**optional**] `jtag-lock-pick_tiny_2` programming probe.

#### Build

```
west build -p -b rpi_pico -s app
```

#### Flash

- Using `jtag-lock-pick_tiny_2`
```
openocd -f 'openocd.cfg' -c 'program path/to/file.hex verify reset exit'
```

See [Zephyr's board documentation](https://docs.zephyrproject.org/latest/boards/arm/rpi_pico/doc/index.html)
for other ways of flashing the board.
