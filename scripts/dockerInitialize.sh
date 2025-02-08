#!/bin/bash

# Define paths
ENV_FILE="Embedded-Sharepoint/.env"
DOCKER_COMPOSE_FILE="Embedded-Sharepoint/docker-compose.yml"

# Create .env file with required environment variables
echo "WORKDIR_MOUNT=${localWorkspaceFolder}" > $ENV_FILE
echo "REMOTE_USER=$USER" >> $ENV_FILE
echo "REMOTE_UID=$(id -u)" >> $ENV_FILE
echo "REMOTE_GID=$(id -g)" >> $ENV_FILE

# Bring down the docker-compose setup
docker compose -f $DOCKER_COMPOSE_FILE down
