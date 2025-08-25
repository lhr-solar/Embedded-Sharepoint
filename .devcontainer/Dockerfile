# Copyright (c) 2025, Longhorn Racing Solar 
# LICENSE in ../LICENSE

ARG TARGETPLATFORM=linux/amd64

# ----------- Base stage -----------
FROM --platform=${TARGETPLATFORM} debian:bookworm-slim AS base

ENV DEBIAN_FRONTEND=noninteractive \
    TZ=UTC \
    HOME=/root

# ----------- System dependencies stage -----------
FROM base AS sys-deps
LABEL stage="system dependencies"

RUN apt-get update && \
    # Generate locales because perl tweaks otherwise
    apt-get install -y locales && \
    sed -i '/en_US.UTF-8/s/^# //g' /etc/locale.gen && \
    locale-gen en_US.UTF-8 && \
    update-locale LANG=en_US.UTF-8 && \
    rm -rf /var/lib/apt/lists/
    #apt-get upgrade -y && \  NOT NEEDED, slows down builds
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential git cmake make bear doxygen \
    gdb-multiarch openocd stlink-tools picocom \
        python3 python3-pip python3-dev wget curl \
        ca-certificates usbutils iputils-ping openssh-client udev \
        file vim nano tree htop parallel  dos2unix sudo \
    && rm -rf /var/lib/apt/lists/*


# ----------- User setup stage -----------
FROM sys-deps AS user-setup
LABEL stage="user setup"

RUN groupadd -r dev && useradd -m -s /bin/bash -g dev dev && \
    usermod -aG dialout,plugdev dev && \
    echo "dev ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/dev && \
    chmod 440 /etc/sudoers.d/dev


# ----------- ARM gcc toolchain install stage -----------
FROM user-setup AS toolchain

LABEL stage="arm gcc toolchain"

ARG GCC_ARM_VERSION=14.3.rel1
WORKDIR /tmp

ARG TARGETPLATFORM
RUN ARCH=$([ "${TARGETPLATFORM}" = "linux/arm64" ] && echo "aarch64" || echo "x86_64") && \
    TOOLCHAIN="arm-gnu-toolchain-${GCC_ARM_VERSION}-${ARCH}-arm-none-eabi.tar.xz" && \
    echo "Downloading ${TOOLCHAIN}" && \
    wget -q "https://developer.arm.com/-/media/Files/downloads/gnu/${GCC_ARM_VERSION}/binrel/${TOOLCHAIN}" && \
    echo "Extracting ${TOOLCHAIN}" && \
    tar -xf "${TOOLCHAIN}" -C /usr/local --strip-components=1 && \
    echo "Extracted to ${pwd}" && \
    rm "${TOOLCHAIN}" && \
    echo "Creating symlinks to /usr/bin" && \
    for bin in arm-none-eabi-gcc arm-none-eabi-g++ arm-none-eabi-objcopy arm-none-eabi-objdump; do \
        ln -sf /usr/local/bin/$bin /usr/bin/$bin; \
    done && \
    mkdir -p /lib/${ARCH}-linux-gnu/ && \
    ln -sf /usr/lib/${ARCH}-linux-gnu/libncurses.so.6 /lib/${ARCH}-linux-gnu/libncurses.so.5 && \
    echo "ARM GCC toolchain installed successfully."

ENV PATH="/usr/local/bin:${PATH}"

# ----------- Python environment stage -----------
FROM toolchain AS python

LABEL stage="python environment"

RUN python3 -m pip install --break-system-packages uv && \
    uv venv --python python3 /home/dev/.venv && \
    chown -R dev:dev /home/dev/.venv

ENV PATH="/home/dev/.venv/bin:${PATH}"

# ----------- Final devcontainer stage -----------
FROM python AS devcontainer

LABEL stage="final devcontainer"

WORKDIR /workdir

COPY .devcontainer/setup.sh /workdir/.devcontainer/setup.sh
RUN chmod +x /workdir/.devcontainer/setup.sh && \
    dos2unix /workdir/.devcontainer/setup.sh && \
    chown -R dev:dev /workdir

USER dev
ENV HOME=/home/dev

CMD ["/bin/bash"]
