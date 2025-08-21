# Use platform-qualified base
ARG TARGETPLATFORM
FROM --platform=${TARGETPLATFORM} ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC

# Consistent dev user
RUN groupadd -g 1000 dev \
 && useradd -u 1000 -g dev -m -s /bin/bash dev \
 && echo "dev ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Install pinned packages
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential=12.9ubuntu3 \
    git=1:2.34.1-1ubuntu1.10 \
    wget=1.21.2-2ubuntu1 \
    curl=7.81.0-1ubuntu1.15 \
    cmake=3.22.1-1ubuntu1.22.04.1 \
    # ninja-build=1.10.1-1 \
    make=4.3-4.1build1 \
    gdb-multiarch=12.1-0ubuntu1~22.04 \
    openocd=0.11.0-1ubuntu1 \
    stlink-tools=1.7.0+ds-1 \
    picocom=3.1-2 \
    python3=3.10.6-1~22.04 \
    python3-pip=22.0.2+dfsg-1ubuntu0.4 \
    python3-dev=3.10.6-1~22.04 \
    net-tools=1.60+git20181103.0eebece-1ubuntu5 \
    iputils-ping=3:20211215-1 \
    dnsutils=1:9.18.18-0ubuntu0.22.04.2 \
    tcpdump=4.99.1-3ubuntu0.1 \
    # netcat-openbsd=1.218-4ubuntu1 \
    openssh-client=1:8.9p1-3ubuntu0.6 \
    ca-certificates=20230311ubuntu0.22.04.1 \
    usbutils=1:014-1 \
    libusb-1.0-0-dev=2:1.0.25-1ubuntu2 \
    libftdi1-dev=1.5-5build1 \
    pkg-config=0.29.2-1ubuntu3 \
    libtool=2.4.6-15build2 \
    autoconf=2.71-2 \
    automake=1:1.16.5-1.3 \
    texinfo=6.8-4build1 \
    flex=2.6.4-8build2 \
    bison=2:3.8.2+dfsg-1build1 \
    bear=3.0.18-1build1 \
    vim=2:8.2.3458-2ubuntu2.4 \
    nano=6.2-1 \
    tree=2.0.2-1 \
    htop=3.0.5-7build2 \
    parallel=20210822+ds-2 \
 && rm -rf /var/lib/apt/lists/*

# Install ARM toolchain
ARG GCC_ARM_VERSION=14.2.rel1
WORKDIR /tmp
RUN case "${TARGETPLATFORM}" in \
        "linux/amd64") ARCH="x86_64" ;; \
        "linux/arm64") ARCH="aarch64" ;; \
    esac \
 && wget -q "https://developer.arm.com/-/media/Files/downloads/gnu/${GCC_ARM_VERSION}/binrel/arm-gnu-toolchain-${GCC_ARM_VERSION}-${ARCH}-arm-none-eabi.tar.xz" \
 && tar -xf arm-gnu-toolchain-${GCC_ARM_VERSION}-${ARCH}-arm-none-eabi.tar.xz -C /usr/local --strip-components=1 \
 && rm arm-gnu-toolchain-${GCC_ARM_VERSION}-${ARCH}-arm-none-eabi.tar.xz \
 && ln -sf /usr/local/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc \
 && ln -sf /usr/local/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++ \
 && ln -sf /usr/local/bin/arm-none-eabi-objcopy /usr/bin/arm-none-eabi-objcopy \
 && ln -sf /usr/local/bin/arm-none-eabi-objdump /usr/bin/arm-none-eabi-objdump \
 && rm -rf /tmp

# Install Python requirements
COPY requirements.txt /tmp/
RUN pip3 install --no-cache-dir -r /tmp/requirements.txt \
 && rm /tmp/requirements.txt

# Fix ncurses
RUN ln -sf /usr/lib/x86_64-linux-gnu/libncurses.so.6 /usr/lib/x86_64-linux-gnu/libncurses.so.5

# Setup workspace
RUN mkdir -p /workdir/{build,tools} && chown -R dev:dev /workdir

USER dev
WORKDIR /workdir

# Add utility scripts
COPY .devcontainer/setup.sh /workdir/.devcontainer/setup.sh
RUN chmod +x /workdir/.devcontainer/setup.sh

CMD ["/bin/bash"]
