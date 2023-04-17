# STM32 quadcopter controller software

Quadcopter controller software for STM32 based hardware.
Project in development stage.
Currently only STM32F303RE Nucleo (NUCLEO-F303RE) development board is supported.

## Development

### STM32 Nucleo F303RE

#### Build

```
west build -p -b nucleo_f303re -s app
```

#### Flash

```
west flash
```

#### Troubleshooting

1. Flashing firmware using `west flash` fails with `Error: read_memory: failed to read memory`

Hold RESET button on nucleo board while running `west flash` command. Next flash should succeed.
