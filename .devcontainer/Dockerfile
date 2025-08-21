# Copyright (c) 2025, Longhorn Racing Solar 
# For more information, see LICENSE file (../LICENSE)

ARG TARGETPLATFORM=linux/amd64
FROM --platform=${TARGETPLATFORM} ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC
ENV HOME=/root

LABEL stage="Create user"
RUN if ! getent group dev >/dev/null; then \
      groupadd dev; \
    fi \
 && if ! id dev >/dev/null 2>&1; then \
      useradd -g dev -m -s /bin/bash dev; \
    fi \
 && echo "dev ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers \
 && usermod -aG dialout,plugdev dev

LABEL stage="Install system dependencies"
RUN apt-get update \
 && apt-get install -y --no-install-recommends \
    build-essential \
    git \
    wget \
    curl \
    cmake \
    make \
    gdb-multiarch \
    openocd \
    stlink-tools \
    picocom \
    python3 \
    python3-pip \
    python3-dev \
    net-tools \
    iputils-ping \
    dnsutils \
    tcpdump \
    openssh-client \
    ca-certificates \
    usbutils \
    libusb-1.0-0-dev \
    libftdi1-dev \
    pkg-config \
    libtool \
    autoconf \
    automake \
    texinfo \
    flex \
    bison \
    bear \
    vim \
    nano \
    tree \
    htop \
    parallel \
    udev \
    dos2unix \
 && rm -rf /var/lib/apt/lists/*

LABEL stage="Install ARM GCC toolchain"
ARG GCC_ARM_VERSION=14.3.rel1
WORKDIR /tmp
RUN if [ "${TARGETPLATFORM}" = "linux/arm64" ]; then \
      ARCH="aarch64"; \
    else \
      ARCH="x86_64"; \
    fi \
 && echo "Installing ARM GCC toolchain version ${GCC_ARM_VERSION} for ${ARCH}..." \
 && wget -q "https://developer.arm.com/-/media/Files/downloads/gnu/${GCC_ARM_VERSION}/binrel/arm-gnu-toolchain-${GCC_ARM_VERSION}-${ARCH}-arm-none-eabi.tar.xz" \
 && if [ ! -f "arm-gnu-toolchain-${GCC_ARM_VERSION}-${ARCH}-arm-none-eabi.tar.xz" ]; then \
      echo "[ERROR]: ARM GCC toolchain archive not found!"; \
      exit 1; \
    fi \
 && tar -xf "arm-gnu-toolchain-${GCC_ARM_VERSION}-${ARCH}-arm-none-eabi.tar.xz" -C /usr/local --strip-components=1 \
 && rm "arm-gnu-toolchain-${GCC_ARM_VERSION}-${ARCH}-arm-none-eabi.tar.xz" \
 && ln -sf /usr/local/bin/arm-none-eabi-gcc       /usr/bin/arm-none-eabi-gcc \
 && ln -sf /usr/local/bin/arm-none-eabi-g++       /usr/bin/arm-none-eabi-g++ \
 && ln -sf /usr/local/bin/arm-none-eabi-objcopy   /usr/bin/arm-none-eabi-objcopy \
 && ln -sf /usr/local/bin/arm-none-eabi-objdump   /usr/bin/arm-none-eabi-objdump \
 && ln -sf /lib/${ARCH}-linux-gnu/libncurses.so.6 /lib/${ARCH}-linux-gnu/libncurses.so.5 \
 && echo "ARM GCC toolchain installed and symlinks resolved successfully."


LABEL stage="Setup workdir and python environment"
RUN mkdir -p /workdir \
 && chown -R dev:dev /workdir
WORKDIR /workdir
# Install uv (Astral-UV) standalone Python environment manager
RUN python3 -m pip install --break-system-packages uv
# Install Python dependencies (i wanted to use uv)
COPY requirements.txt /workdir/requirements.txt
RUN cd /workdir \
 && uv venv --python python3 \
 && uv pip sync requirements.txt \
 && rm requirements.txt
ENV PATH="/workdir/.venv/bin:${PATH}"

LABEL stage="Copy setup script"
COPY .devcontainer/setup.sh /workdir/.devcontainer/setup.sh
RUN chmod +x /workdir/.devcontainer/setup.sh \
  && dos2unix /workdir/.devcontainer/setup.sh

USER dev
ENV HOME=/home/dev
CMD ["/bin/bash"]