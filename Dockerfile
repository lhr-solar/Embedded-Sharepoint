# Dockerfile for Embedded-Sharepoint
FROM ubuntu:22.04 AS base

# non-interactive mode for apt-get
ENV DEBIAN_FRONTEND=noninteractive

# Use bash for subsequent RUN commands
SHELL ["/bin/bash", "-c"]

# -----------------------------------------------------------------------------
# ARGs
# -----------------------------------------------------------------------------
ARG ARM_GNU_VERSION=14.2
ARG ARM_GNU_TARBALL_URL="https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz"

# -----------------------------------------------------------------------------
# Update system package repositories
# -----------------------------------------------------------------------------
RUN apt-get update && \
    apt-get install software-properties-common -y &&\
    add-apt-repository ppa:deadsnakes/ppa -y && \
    apt-get update

FROM base AS dev
# -----------------------------------------------------------------------------
# Install base dependencies
# -----------------------------------------------------------------------------
RUN apt-get install -y --no-install-recommends \
      # base packages
      vim \
      wget \
      build-essential \
      gdb-multiarch \
      openocd \
      stlink-tools \
      libncurses-dev \
      python3-pip \
      git \
      gh \
      gnupg \
      dirmngr \
      bear \
      ca-certificates \
      openssh-client \
      python3.10 \
      python3.10-dev \
      usbutils \
    && update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.10 1 \
    && rm -rf /var/lib/apt/lists/*

# -----------------------------------------------------------------------------
# Install Python
# -----------------------------------------------------------------------------
RUN pip install --no-cache-dir \
    mkdocs

FROM dev AS toolchain-cache
# -----------------------------------------------------------------------------
# Single RUN step for (1) ARM Toolchain, (3) ncurses symlinks, 
# and (4) LLVM/Clang
# -----------------------------------------------------------------------------

# Use ADD to add these to the build cache
ADD $ARM_GNU_TARBALL_URL /tmp/arm-none-eabi.tar.xz
ADD https://apt.llvm.org/llvm.sh /tmp/llvm.sh

FROM toolchain-cache AS toolchain
RUN \
  # -----------------------------------------------------------
  # 1) ARM None EABI Toolchain
  # -----------------------------------------------------------
  # wget -O /tmp/arm-none-eabi.tar.xz "$ARM_GNU_TARBALL_URL" && \
  tar -xf /tmp/arm-none-eabi.tar.xz -C /usr/share/ && \
  mv /usr/share/arm-gnu-toolchain-${ARM_GNU_VERSION}.rel1-x86_64-arm-none-eabi /usr/share/arm-gnu-toolchain && \
  ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-gcc    /usr/bin/arm-none-eabi-gcc && \
  ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-g++    /usr/bin/arm-none-eabi-g++ && \
  ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-objcopy /usr/bin/arm-none-eabi-objcopy && \
  ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-size    /usr/bin/arm-none-eabi-size && \
  ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-gdb     /usr/bin/arm-none-eabi-gdb && \
  rm /tmp/arm-none-eabi.tar.xz && \
  \
  # -----------------------------------------------------------
  # 2) Ncurses symlinks
  # -----------------------------------------------------------
  ln -sf /usr/lib/x86_64-linux-gnu/libncurses.so.6  /usr/lib/x86_64-linux-gnu/libncurses.so.5 && \
  ln -sf /usr/lib/x86_64-linux-gnu/libncursesw.so.6 /usr/lib/x86_64-linux-gnu/libncursesw.so.5 && \
  ln -sf /usr/lib/x86_64-linux-gnu/libtinfo.so.6    /usr/lib/x86_64-linux-gnu/libtinfo.so.5 && \
  \
  # -----------------------------------------------------------
  # 3) LLVM/Clang 17
  # -----------------------------------------------------------
  # wget https://apt.llvm.org/llvm.sh && \
  chmod +x /tmp/llvm.sh && \
  /tmp/llvm.sh 17 all && \
  apt-get install -y --no-install-recommends gcc-multilib && \
  rm /tmp/llvm.sh

FROM toolchain AS runtime
# -----------------------------------------------------------------------------
# Set working directory
# -----------------------------------------------------------------------------
WORKDIR /workdir

# -----------------------------------------------------------------------------
# Create a non-root user
# -----------------------------------------------------------------------------
ARG REMOTE_USER=dev
ARG REMOTE_UID=1000
ARG REMOTE_GID=1000

RUN addgroup --gid ${REMOTE_GID} ${REMOTE_USER} && \
    adduser --disabled-password --gecos "" --uid ${REMOTE_UID} --gid ${REMOTE_GID} ${REMOTE_USER}

ENV HOME /home/${REMOTE_USER}

USER ${REMOTE_USER}

# -----------------------------------------------------------------------------
# Shell customization
# -----------------------------------------------------------------------------
RUN echo 'export PS1="\[\e[0;33m\]LHRS@\[\e[1;34m\]\w\[\e[0m\]> "' \
    >> /home/${REMOTE_USER}/.bashrc && \
    echo 'echo -e "${RED}\nFinished! Jolly good!\n${NC}"' \
    >> /home/${REMOTE_USER}/.bashrc