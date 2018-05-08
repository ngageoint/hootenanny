#!/bin/bash
set -e

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm

hoot name-frequencies $inputfile | LANG=C sort
