#!/bin/bash
set -e

CONFIG="--warn -C Testing.conf"

hoot tag-info $CONFIG $HOOT_HOME/test-files/jakarta_raya_coastline.shp

hoot tag-info $CONFIG $HOOT_HOME/test-files/jakarta_raya_coastline.shp --tag-values-limit 1

hoot tag-info $CONFIG $HOOT_HOME/test-files/DcTigerRoads.osm --tag-values-limit 5

hoot tag-info $CONFIG $HOOT_HOME/test-files/DcTigerRoads.osm --keys-only

hoot tag-info $CONFIG $HOOT_HOME/test-files/DcTigerRoads.osm --keys "foot;highway"

hoot tag-info $CONFIG $HOOT_HOME/test-files/DcTigerRoads.osm --keys "foot;HIGHWAY"

hoot tag-info $CONFIG $HOOT_HOME/test-files/DcTigerRoads.osm --keys "foot;HIGHWAY" --case-insensitive

hoot tag-info $CONFIG $HOOT_HOME/test-files/DcTigerRoads.osm --keys "blah"

hoot tag-info $CONFIG $HOOT_HOME/test-files/DcTigerRoads.osm --keys "high" --partial-key-match

hoot tag-info $CONFIG $HOOT_HOME/test-files/DcTigerRoads.osm --keys "HIGH" --partial-key-match

hoot tag-info $CONFIG $HOOT_HOME/test-files/DcTigerRoads.osm --keys "HIGH" --partial-key-match --case-insensitive
