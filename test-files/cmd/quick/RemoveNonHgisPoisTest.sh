#!/bin/bash

mkdir -p test-output/cmd/quick/
scripts/RemoveNonHgisPois.js test-files/conflate/unified/AllDataTypesA.osm test-output/cmd/quick/RemoveNonHgisPoisTest.osm
hoot stats --quick test-output/cmd/quick/RemoveNonHgisPoisTest.osm | grep -v ID
