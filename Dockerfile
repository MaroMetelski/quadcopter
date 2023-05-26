FROM zephyrprojectrtos/zephyr-build:v0.26.1

ARG GO_VERSION=1.20.4

# Download and install Go
WORKDIR /tmp
RUN wget https://dl.google.com/go/go$GO_VERSION.linux-amd64.tar.gz && \
    sudo tar -C /usr/local/ -xzf go$GO_VERSION.linux-amd64.tar.gz
ENV PATH=$PATH:/usr/local/go/bin:/home/user/go/bin

# This will be properly set when running west init and west update
ENV ZEPHYR_BASE=

# Install development tools
RUN go install github.com/go-task/task/v3/cmd/task@latest

WORKDIR /workdir
