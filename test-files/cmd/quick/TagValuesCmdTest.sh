#!/bin/bash
set -e

hoot tag-values --warn $HOOT_HOME/test-files/jakarta_raya_coastline.shp

hoot tag-values --warn -D tag.values.max.tag.values.per.tag.key=1 $HOOT_HOME/test-files/jakarta_raya_coastline.shp

hoot tag-values --warn -D tag.values.max.tag.values.per.tag.key=5 $HOOT_HOME/test-files/DcGisRoads.osm
