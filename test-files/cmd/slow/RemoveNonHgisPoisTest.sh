#!/bin/bash

mkdir -p test-output/cmd/slow/
scripts/RemoveNonHgisPois.js test-files/conflate/unified/AllDataTypesA.osm test-output/cmd/slow/RemoveNonHgisPoisTest.osm
hoot stats --quick test-output/cmd/slow/RemoveNonHgisPoisTest.osm | grep -v ID
