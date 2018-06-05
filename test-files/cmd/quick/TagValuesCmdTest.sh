#!/bin/bash
set -e

hoot tag-values --warn $HOOT_HOME/test-files/jakarta_raya_coastline.shp

hoot tag-values --warn $HOOT_HOME/test-files/jakarta_raya_coastline.shp --tag-values-limit 1

hoot tag-values --warn $HOOT_HOME/test-files/DcGisRoads.osm --tag-values-limit 5
