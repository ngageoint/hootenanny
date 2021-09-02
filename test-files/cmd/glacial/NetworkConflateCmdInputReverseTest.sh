#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/glacial/NetworkConflateCmdInputReverseTest

# Let's see if we can conflate the other way, since that wasn't working at one point.
# The output is different, which is a little disturbing, but we'll deal with that later.
hoot conflate --warn -C ReferenceConflation.conf -C NetworkAlgorithm.conf -C Testing.conf \
 -D match.creators=NetworkMatchCreator -D merger.creators=NetworkMergerCreator \
 test-files/DcTigerRoads.osm test-files/DcGisRoads.osm \
 test-output/cmd/glacial/NetworkConflateCmdInputReverseTest/output.osm

hoot diff -C Testing.conf test-output/cmd/glacial/NetworkConflateCmdInputReverseTest/output.osm \
          test-files/cmd/glacial/NetworkConflateCmdInputReverseTest/output.osm || \
     diff test-output/cmd/glacial/NetworkConflateCmdInputReverseTest/output.osm \
          test-files/cmd/glacial/NetworkConflateCmdInputReverseTest/output.osm
