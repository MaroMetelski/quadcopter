# STM32 quadcopter controller software

Quadcopter controller software for STM32 based hardware.
Project in development stage.
Currently only STM32F303RE Nucleo (NUCLEO-F303RE) development board is supported.

## Environment set up

### Application

1. [Install Zephyr dependencies](https://docs.zephyrproject.org/latest/develop/getting_started/index.html#install-dependencies)
2. Get the source code
```
git clone https://github.com/MaroMetelski/quadcopter.git
```
3. Install `west` and initialize Zephyr application
```
cd quadcopter
west init -l app
west update
```

### Unit tests

2. Install ThrowTheSwitch C unit test framework (Ceedling, Unity and CMock)

Follow [official instructions](http://www.throwtheswitch.org/ceedling) to install Ceedling.
This will contain all necessary dependencies and will allow you to run test commands.

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

### Unit tests

Unit tests are built using [Unity](https://github.com/ThrowTheSwitch/Unity) framework
with [CMock](https://github.com/ThrowTheSwitch/CMock)
and run using [ceedling](https://github.com/ThrowTheSwitch/Ceedling).

#### Execute tests

Execute unit tests placed in `tests/unit`
```
ceedling test:all
```
