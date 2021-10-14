#!/bin/bash
set -e


# Really basic "smoke test" for the container
# NOTE: You must have the directory with the datafiles mounted onto the container or this will fail

curl -s -X POST -H 'Content-Type: application/json' \
  -d '{"args": ["/hoot_data/ToyTestA.osm", "/hoot_data/ToyTestB.osm", "/hoot_data/ToyOutput.osm"]}' \
  http://127.0.0.1:5000/scripts/RunCommandsAcrossContainers

