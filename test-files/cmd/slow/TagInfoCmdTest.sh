#!/bin/bash
set -e

hoot tag-info --warn $HOOT_HOME/test-files/jakarta_raya_coastline.shp

hoot tag-info --warn $HOOT_HOME/test-files/jakarta_raya_coastline.shp --tag-values-limit 1

hoot tag-info --warn $HOOT_HOME/test-files/DcTigerRoads.osm --tag-values-limit 5

hoot tag-info --warn $HOOT_HOME/test-files/DcTigerRoads.osm --keys-only

hoot tag-info --warn $HOOT_HOME/test-files/DcTigerRoads.osm --keys "foot;highway"

hoot tag-info --warn $HOOT_HOME/test-files/DcTigerRoads.osm --keys "blah"
