#!/bin/bash

mkdir -p test-output/cmd/quick/
scripts/services/RemoveNonHgisPois.js test-files/conflate/unified/AllDataTypesA.osm test-output/cmd/quick/RemoveNonHgisPoisTest.osm
hoot stats --brief test-output/cmd/quick/RemoveNonHgisPoisTest.osm | grep -v ID
