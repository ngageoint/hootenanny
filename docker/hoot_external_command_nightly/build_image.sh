#!/usr/bin/env bash

set -e

# Really basic script to build a container
IMGNAME=hoot_external_command
IMGVER=0.1

BASEDIR=$(dirname "$0")

if [[ "$(docker images -q ${IMGNAME}:latest)" == "" ]]; then
  echo "Building the image"
  docker build \
  	-t ${IMGNAME}:${IMGVER} ${BASEDIR}

  echo "Tagging the new image"
  docker tag ${IMGNAME}:${IMGVER} ${IMGNAME}:latest
else
  echo "Image exists: ${IMGNAME}:latest"
fi
