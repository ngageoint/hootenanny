#!/bin/bash
set -e

hoot tag-distribution --warn $HOOT_HOME/test-files/DcGisRoads.osm highway

hoot tag-distribution --warn $HOOT_HOME/test-files/DcGisRoads.osm highway --sort-by-value

hoot tag-distribution --warn $HOOT_HOME/test-files/DcGisRoads.osm highway hoot::HighwayCriterion

hoot tag-distribution --warn $HOOT_HOME/test-files/DcGisRoads.osm highway hoot::HighwayCriterion --sort-by-value

hoot tag-distribution --warn $HOOT_HOME/test-files/DcGisRoads.osm name

hoot tag-distribution --warn $HOOT_HOME/test-files/DcGisRoads.osm name --sort-by-value

hoot tag-distribution --warn $HOOT_HOME/test-files/DcGisRoads.osm name hoot::HighwayCriterion

hoot tag-distribution --warn $HOOT_HOME/test-files/DcGisRoads.osm name hoot::HighwayCriterion --sort-by-value

hoot tag-distribution --warn $HOOT_HOME/test-files/DcGisRoads.osm blah
