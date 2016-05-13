#!/bin/bash
set -e

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm

hoot names $inputfile | LANG=C sort
