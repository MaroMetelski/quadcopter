# quadcopter controller firmware

> **INFO**: Project is in development stage.

Controller firmware for multiple hardware and emulated targets. The project is based on
[Zephyr](https://github.com/zephyrproject-rtos/zephyr). Zephyr solutions are used for the build system, board support and platform code (HAL, subsystems, RTOS). Nonetheless, significant
amount of controller code is platform-agnostic.

## Supported boards

### Native application

Native application build allows to easily test the code on the
developers machine without any extra hardware. Although limited
in IO capabilities, this build is great for testing high-level APIs.

### [stm32 controller board](https://github.com/MaroMetelski/quadcopter-hw)

STM32F3 quadcopter flight controller for controlling simple machines with brushed DC motors.

# Development

## Environment

> **INFO**: The recommended way is to use Docker-based environment and VSCode
devcontainers. Documentation for other set-ups may be incomplete.

### With Docker

The root Dockerfile provides definition for the development image with all tools
preinstalled.
Simply run all the development commands from this documentation inside this
container, for example `docker exec <container_name> <command>`.

You can either build the image yourself or use it together with VSCode as a
devcontainer using Dev Containers plugin. This approach is described in next
section. This is the recommended set-up.
If you want to build and use the image yourself please do at your own discretion.

#### VScode devcontainer

This project includes support for VSCode Dev Container. That means you can
simply:
1. Install Dev Containers extension.
2. Run VSCode command `Dev Containers: Reopen in container` (or
`Dev Containers: Rebuild and reopen in container` if running for the first time).

From now on you will be able to run all the development commands in the built-in
terminal from VSCode.

### Local (not recommended)

> **TODO:** Document local environment set-up.

## Task

This project uses Task for running common development commands.

> Task is a task runner / build tool that aims to be simpler and easier to
use than, for example, GNU Make.

## Initialize Zephyr project

To build and run the Zephyr application you need to set up the project with `Task`, which in turn calls Zephyr's `west` build system.

```
cd quadcopter
task project:init
task project:update
```

## Unit tests

Unit tests are run using ThrowTheSwitch framework consisting of Ceedling, Unity and CMock. Task wrappers are provided
to simplify running tests.

To run all Unit Tests simply call:
```
task test:all
```

## Build and run

### Controller board

#### Build

Build application for controller board using:
```
task build:controller
```

#### Flash

Flash controller board with:
```
west flash
```

### Native application

#### Build

Build native application using:
```
task build:native
```

#### Run

Run native application with:
```
west build -t run
```
