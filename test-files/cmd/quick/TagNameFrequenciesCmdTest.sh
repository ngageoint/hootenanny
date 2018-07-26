#!/bin/bash
set -e

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm

hoot tag-name-frequencies $inputfile | LC_ALL=C sort
