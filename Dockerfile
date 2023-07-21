FROM zephyrprojectrtos/zephyr-build:v0.26.4

ARG GO_VERSION=1.20.4
ARG ZSDK_VERSION=0.16.1

# Download and install Go
WORKDIR /tmp
RUN wget https://dl.google.com/go/go$GO_VERSION.linux-amd64.tar.gz && \
    sudo tar -C /usr/local/ -xzf go$GO_VERSION.linux-amd64.tar.gz
ENV PATH=$PATH:/usr/local/go/bin:/home/user/go/bin

# This will be properly set when running west init and west update
ENV ZEPHYR_BASE=

# Install development tools
RUN go install github.com/go-task/task/v3/cmd/task@latest

# Change to root to install system packages
USER root

# Install development system packages

# FIXME: 
# "The repository 'http://apt.llvm.org/jammy jammy Release' does not have a Release file"
RUN add-apt-repository -r \ 
    http://apt.llvm.org/jammy llvm-toolchain-jammy-16 Release

RUN apt-get -y update && \
    apt-get -y upgrade && \
    apt-get -y install \
        udev
# Copy OpenOCD udev rules
RUN cp \
    /opt/toolchains/zephyr-sdk-${ZSDK_VERSION}/sysroots/x86_64-pokysdk-linux/usr/share/openocd/contrib/60-openocd.rules \
    /etc/udev/rules.d
# Back to 'user'
USER user

WORKDIR /workdir
