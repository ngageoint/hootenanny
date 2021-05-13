#!/usr/bin/env bash

# Very basic script to build and run a Hootenanny comandline via Docker

set -e


BASEDIR=$(dirname "$0")

echo "Running in: $BASEDIR"
#################################################

echo "Build the docker image"
$BASEDIR/docker/hoot_commandline/build_image.sh

# run the image
echo "Run the container"
$BASEDIR/docker/hoot_commandline/run_image.sh

