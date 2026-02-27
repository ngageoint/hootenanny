#!/usr/bin/env bash

set -e

# Push all CircleCI EL9 Docker images to Docker Hub

echo "=========================================="
echo "Pushing all CircleCI EL9 Docker images"
echo "=========================================="
echo ""

# Check if logged in to Docker Hub
if ! docker info | grep -q "Username"; then
    echo "Please login to Docker Hub first:"
    docker login
fi

echo ""
echo "Pushing images to Docker Hub..."
echo ""

# Push rpmbuild-hoot-release-el9
echo "1/3: Pushing rpmbuild-hoot-release-el9..."
docker push hootenanny/rpmbuild-hoot-release-el9:latest
echo ""

# Push rpmbuild-repo-el9
echo "2/3: Pushing rpmbuild-repo-el9..."
docker push hootenanny/rpmbuild-repo-el9:latest
echo ""

# Push rpmbuild-generic-el9
echo "3/3: Pushing rpmbuild-generic-el9..."
docker push hootenanny/rpmbuild-generic-el9:latest
echo ""

echo "=========================================="
echo "All images pushed successfully!"
echo "=========================================="
echo ""
echo "Images available on Docker Hub:"
echo "  - hootenanny/rpmbuild-hoot-release-el9:latest"
echo "  - hootenanny/rpmbuild-repo-el9:latest"
echo "  - hootenanny/rpmbuild-generic-el9:latest"
echo ""
echo "CircleCI can now use these images."

