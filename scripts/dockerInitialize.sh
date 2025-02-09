#!/bin/bash

# Define paths relative to the script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ENV_FILE="$(dirname "$script_dir")/../.env"
DOCKER_COMPOSE_FILE="$(dirname "$script_dir")/../docker-compose.yml"


# Create .env file with required environment variables
echo "WORKDIR_MOUNT=${localWorkspaceFolder}" > $ENV_FILE
echo "REMOTE_USER=$USER" >> $ENV_FILE
echo "REMOTE_UID=$(id -u)" >> $ENV_FILE
echo "REMOTE_GID=$(id -g)" >> $ENV_FILE

# Need to bring down compose to prevent using other projects compose files
docker compose -f $DOCKER_COMPOSE_FILE down
