#!/usr/bin/env bash
#
# start.sh
#
# This script automates environment setup for embedded-sharepoint

IMAGE_NAME="embedded-sharepoint"
if [ -z "$1" ]; then
  export WORKDIR_MOUNT="$PWD"
else
  export WORKDIR_MOUNT="$1"
fi
DOCKERFILE_DIR="$(dirname "$0")"
DOCKERFILE_PATH="$DOCKERFILE_DIR/Dockerfile"
DOCKER_COMPOSE_FILE="$DOCKERFILE_DIR/docker-compose.yml"

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
  
  # Add Docker's official GPG key:
  sudo apt-get update
  sudo apt-get install ca-certificates curl
  sudo install -m 0755 -d /etc/apt/keyrings
  sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
  sudo chmod a+r /etc/apt/keyrings/docker.asc

  # Add the repository to Apt sources:
  echo \
    "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
    $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
    sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
  sudo apt-get update

  sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
  
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
  echo "[ERROR] Docker is installed but not running. Please start Docker \(e.g., 'sudo systemctl start docker'\)."
  exit 1
fi

###############################################################################
# Run the container
###############################################################################
echo "[INFO] Running the Docker container ..."
echo "      - Image: $IMAGE_NAME"
echo "      - Mount: $WORKDIR_MOUNT -> /Embedded-Sharepoint"
echo "-----------------------------------------------------"

# Sets SSH_AUTH_SOCK for macOS
if [[ "$OSTYPE" == "darwin"* ]]; then
  export SSH_AUTH_SOCK="/run/host-services/ssh-auth.sock"
fi

# Start ssh-agent
# test whether $SSH_AUTH_SOCK is valid
ssh-add -l 2>/dev/null >/dev/null
# if not valid, then start ssh-agent using $SSH_AUTH_SOCK
[ $? -ge 2 ] && ssh-agent -a "$SSH_AUTH_SOCK" >/dev/null

export REMOTE_USER="$USER"
export REMOTE_UID=$(id -u)
export REMOTE_GID=$(id -g)
docker compose -f "$DOCKER_COMPOSE_FILE" build
docker compose -f "$DOCKER_COMPOSE_FILE" run --rm dev

echo "[INFO] Container exited."
