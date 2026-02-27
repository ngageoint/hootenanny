#!/usr/bin/env bash

set -e

# Build script for CircleCI RPM build image (Hoot Release) - EL9
IMGNAME=hootenanny/rpmbuild-hoot-release-el9
IMGVER=latest

BASEDIR=$(dirname "$0")

echo "Building CircleCI RPM build image for Hootenanny releases (EL9)..."
docker build -t ${IMGNAME}:${IMGVER} ${BASEDIR}

echo ""
echo "Image built successfully: ${IMGNAME}:${IMGVER}"
echo ""
echo "To push to Docker Hub:"
echo "  docker login"
echo "  docker push ${IMGNAME}:${IMGVER}"
echo ""
echo "To test the image:"
echo "  docker run -it --rm ${IMGNAME}:${IMGVER} /bin/bash"

