#!/usr/bin/env bash

set -e

# Master build script for all CircleCI EL9 Docker images
# This script builds all three required images for CircleCI

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=========================================="
echo "Building all CircleCI EL9 Docker images"
echo "=========================================="
echo ""

# Build rpmbuild-hoot-release-el9
echo "1/3: Building rpmbuild-hoot-release-el9..."
cd "${SCRIPT_DIR}/rpmbuild-hoot-release-el9"
./build_image.sh
echo ""

# Build rpmbuild-repo-el9
echo "2/3: Building rpmbuild-repo-el9..."
cd "${SCRIPT_DIR}/rpmbuild-repo-el9"
./build_image.sh
echo ""

# Build rpmbuild-generic-el9
echo "3/3: Building rpmbuild-generic-el9..."
cd "${SCRIPT_DIR}/rpmbuild-generic-el9"
./build_image.sh
echo ""

echo "=========================================="
echo "All images built successfully!"
echo "=========================================="
echo ""
echo "Images created:"
echo "  - hootenanny/rpmbuild-hoot-release-el9:latest"
echo "  - hootenanny/rpmbuild-repo-el9:latest"
echo "  - hootenanny/rpmbuild-generic-el9:latest"
echo ""
echo "To push all images to Docker Hub:"
echo "  docker login"
echo "  docker push hootenanny/rpmbuild-hoot-release-el9:latest"
echo "  docker push hootenanny/rpmbuild-repo-el9:latest"
echo "  docker push hootenanny/rpmbuild-generic-el9:latest"
echo ""
echo "Or use the push script:"
echo "  ${SCRIPT_DIR}/push_all_el9_images.sh"

