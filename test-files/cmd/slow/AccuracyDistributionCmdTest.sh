#!/bin/bash
set -e

#mkdir -p test-output/cmd/AccuracyDistributionCmdTest

hoot accuracy-dist test-files/DcGisRoads.osm

# only stderr & stdout are checked, so no need to add anything here
