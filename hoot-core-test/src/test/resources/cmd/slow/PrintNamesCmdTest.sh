#!/bin/bash
set -e

#mkdir -p $HOOT_HOME/test-output/cmd/slow/

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm

hoot names $inputfile
