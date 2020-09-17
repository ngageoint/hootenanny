#!/usr/bin/env bash

set -e

# Really basic script to build a container
IMGNAME=hoot_core
IMGVER=0.1

# Uncoment this to re-use the same id
# echo "Removing the old image"
# docker images | grep ${IMGNAME}:latest | awk '{print $1":"$2}' | xargs --no-run-if-empty docker rmi

echo "Build the image"
docker build \
	--build-arg DEV_UID=$(id -u) \
	--build-arg DEV_GID=$(id -g) \
	-t ${IMGNAME}:${IMGVER} .

echo "Tagging the new image"
docker tag ${IMGNAME}:${IMGVER} ${IMGNAME}:latest

