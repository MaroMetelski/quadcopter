# Bare minimum Zephyr application

Quickly set up bare minimum project with flat workspace and single application.
Build from this sample for your use case. You need to know what you are doing after the setup.
This assumes SDK and all prerequisites are met. See Zephyr's [Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html).

```bash
# get the sample
$ git clone https://github.com/MaroMetelski/zephyr-app.git && cd zephyr-app

# [OPTIONAL] set up python virtual environment (here virtualenvwrapper is used)
$ mkvirtualenv zephyr-app

# Initialize west workspace *below* app. This will place all modules in current directory.
$ pip3 install west
$ west init -l app
$ west update

# build and run qemu_x86 application
$ pip3 install -r zephyr/scripts/requirements.txt
$ west build -s app -b qemu_x86
$ west build -t run

```

*Now you are on your own!*
