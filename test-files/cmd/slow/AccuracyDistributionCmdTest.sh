#!/bin/bash
set -e

# We just run the command, and stdout & stderr 
# get checked for correctness
hoot accuracy-dist test-files/DcGisRoads.osm
