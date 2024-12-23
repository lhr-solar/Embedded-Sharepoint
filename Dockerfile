# Dockerfile for Embedded-Sharepoint
FROM ubuntu:22.04

# non-interactive mode for apt-get
ENV DEBIAN_FRONTEND=noninteractive

# Use bash for subsequent RUN commands
SHELL ["/bin/bash", "-c"]

# ------------------------------------------------------------
# Update system packages and install base dependencies
# ------------------------------------------------------------
RUN apt-get update && apt-get install -y --no-install-recommends \
    vim \
    wget \
    build-essential \
    gdb-multiarch \
    openocd \
    stlink-tools \
    software-properties-common \
    libncurses-dev \
    python3-pip \
    git \
    gnupg \
    dirmngr \
    bear \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# ------------------------------------------------------------
# Python 3.10
# ------------------------------------------------------------
RUN add-apt-repository ppa:deadsnakes/ppa -y && apt-get update

# Install Python 3.10 (system default on jammy) and dev headers
RUN apt-get install -y --no-install-recommends \
    python3.10 python3.10-dev python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Make python3.10 default
RUN update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.10 1

# ------------------------------------------------------------
# Arm None EABI Toolchain
# ------------------------------------------------------------
ARG ARM_GNU_VERSION=14.2
ARG ARM_GNU_TARBALL_URL="https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-x86_64-arm-none-eabi.tar.xz"

RUN wget -O /tmp/arm-none-eabi.tar.xz "$ARM_GNU_TARBALL_URL" \
    && tar -xf /tmp/arm-none-eabi.tar.xz -C /usr/share/ \
    && mv /usr/share/arm-gnu-toolchain-${ARM_GNU_VERSION}.rel1-x86_64-arm-none-eabi /usr/share/arm-gnu-toolchain \
    && ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc \
    && ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++ \
    && ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-objcopy /usr/bin/arm-none-eabi-objcopy \
    && ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-size /usr/bin/arm-none-eabi-size \
    && ln -sf /usr/share/arm-gnu-toolchain/bin/arm-none-eabi-gdb /usr/bin/arm-none-eabi-gdb \
    && rm /tmp/arm-none-eabi.tar.xz

# library symlinks
RUN ln -sf /usr/lib/x86_64-linux-gnu/libncurses.so.6 /usr/lib/x86_64-linux-gnu/libncurses.so.5 \
    && ln -sf /usr/lib/x86_64-linux-gnu/libncursesw.so.6 /usr/lib/x86_64-linux-gnu/libncursesw.so.5 \
    && ln -sf /usr/lib/x86_64-linux-gnu/libtinfo.so.6 /usr/lib/x86_64-linux-gnu/libtinfo.so.5

# ------------------------------------------------------------
# Documentation Toolset
# ------------------------------------------------------------
# Sphinx & Sphinx RTD Theme
RUN apt-get update && apt-get install -y --no-install-recommends \
    python3-sphinx \
    && rm -rf /var/lib/apt/lists/* \
    && pip install --no-cache-dir sphinx_rtd_theme

# Breathe
RUN pip install --no-cache-dir breathe

# Doxygen 
ARG DOXYGEN_VERSION=1.9.7
RUN wget -O /tmp/doxygen.bin.tar.gz "https://github.com/doxygen/doxygen/releases/download/Release_${DOXYGEN_VERSION//./_}/doxygen-${DOXYGEN_VERSION}.linux.bin.tar.gz" \
    && mkdir -p /usr/share/doxygen \
    && tar -xf /tmp/doxygen.bin.tar.gz --strip-components=1 -C /usr/share/doxygen \
    && ln -sf /usr/share/doxygen/bin/doxygen /usr/bin/doxygen \
    && rm /tmp/doxygen.bin.tar.gz

# mkdocs
RUN pip install --no-cache-dir mkdocs

# ------------------------------------------------------------
# LLVM/Clang
# ------------------------------------------------------------
RUN wget https://apt.llvm.org/llvm.sh \
    && chmod +x llvm.sh \
    && ./llvm.sh 17 all \
    && apt-get install -y --no-install-recommends gcc-multilib \
    && rm llvm.sh

# ------------------------------------------------------------
# set working directory
# ------------------------------------------------------------
WORKDIR /Embedded-Sharepoint

RUN echo 'export PS1="\[\e[0;33m\]LHRS@\[\e[1;34m\]Embedded-Sharepoint\[\e[0m\]> "' \
    >> /root/.bashrc
RUN echo 'git config --global --add safe.directory /Embedded-Sharepoint' \
    >> /root/.bashrc
RUN echo 'git submodule update --init --recursive' \
    >> /root/.bashrc
RUN echo 'echo -e "${RED}\nFinished! Jolly good!\n${NC}"' \
    >> /root/.bashrc


# Entry point
CMD ["/bin/bash", "-i"]