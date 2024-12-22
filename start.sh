#!/usr/bin/env bash
#
# start.sh
#
# This script automates environment setup for embedded-sharepoint

IMAGE_NAME="embedded-sharepoint"
WORKDIR_MOUNT="$PWD"
DOCKERFILE_DIR="$(dirname "$0")"

echo "====================================================="
echo " Embedded Sharepoint Setup"
echo "====================================================="

###############################################################################
# Detect Distro
###############################################################################

if [ -f /etc/os-release ]; then
  source /etc/os-release
  DISTRO_ID=$ID
elif [ -f /etc/issue ]; then
  # Fallback for older systems
  DISTRO_ID=$(head -n1 /etc/issue | awk '{print tolower($1)}')
else
  DISTRO_ID="unknown"
fi

###############################################################################
# Check if Docker is installed
###############################################################################

function install_docker_ubuntu() {
  echo "[INFO] Installing Docker for Ubuntu..."
  sudo apt-get update && sudo apt-get install -y docker.io
  sudo systemctl enable docker
  sudo systemctl start docker
}

function install_docker_arch() {
  echo "[INFO] Installing Docker for Arch..."
  sudo pacman -Sy --noconfirm docker
  sudo systemctl enable docker
  sudo systemctl start docker
}

if ! command -v docker &> /dev/null; then
  echo "[INFO] Docker not found on your system."

  if [[ "$DISTRO_ID" == "ubuntu" ]]; then
    install_docker_ubuntu
  elif [[ "$DISTRO_ID" == "arch" ]]; then
    install_docker_arch
  else
    echo "[WARNING] Your distro is '$DISTRO_ID', which isn't supported by this script."
    echo "          Please follow Docker's official instructions at:"
    echo "          https://docs.docker.com/get-docker/"
    exit 1
  fi

  # Add current user to docker group
  echo "[INFO] Adding '$USER' to the 'docker' group..."
  sudo groupadd docker 2> /dev/null || true
  sudo usermod -aG docker "$USER"

  echo "[INFO] Docker installation complete."
  echo "      You may need to log out and log back in (or restart your shell) for the group"
  echo "      membership to take effect. Please do that, then re-run this script."
  exit 0
else
  echo "[OK] Docker is already installed."
fi

# Double-check Docker is running properly
if ! docker ps &> /dev/null; then
  echo "[ERROR] Docker is installed but not running. Please start Docker (e.g., 'sudo systemctl start docker')."
  exit 1
fi

###############################################################################
# Check if the Docker image already exists
###############################################################################
# assume that Dockerfile is in the same directory
echo "[INFO] Checking if the Docker image '$IMAGE_NAME' exists..."

IMAGE_EXISTS=$(docker images -q "$IMAGE_NAME")

if [ -z "$IMAGE_EXISTS" ]; then
    echo "[INFO] '$IMAGE_NAME' not found. Building image from Dockerfile..."
    
    # Build the image
    docker build -t "$IMAGE_NAME" "$DOCKERFILE_DIR"
else
    echo "[OK] Docker image '$IMAGE_NAME' already exists."
fi

###############################################################################
# Run the container with:
#    - USB passthrough 
#    - Current directory mounted at /Embedded-Sharepoint
#    - Interactive shell
###############################################################################
echo "[INFO] Running the Docker container ..."
echo "      - Image: $IMAGE_NAME"
echo "      - Mount: $WORKDIR_MOUNT -> /Embedded-Sharepoint"
echo "-----------------------------------------------------"

# Flags:
# --rm           : Remove container after it exits
# -it            : Interactive mode, attach terminal
# --privileged   : Simplifies USB passthrough on WSL2, 
# -v $WORKDIR_MOUNT:/Embedded-Sharepoint
#                : Mount the current directory
# -w /workdir    : Start in /Embedded-Sharepoint inside the container
# $IMAGE_NAME    : Docker image tag
docker run --rm -it \
    --privileged \
    -v "${WORKDIR_MOUNT}:/Embedded-Sharepoint" \
    -w /Embedded-Sharepoint\
    "${IMAGE_NAME}" \
    /bin/bash

echo "[INFO] Container exited."
