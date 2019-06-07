#!/bin/bash
set -e

# See ticket #6208 for an explanation

mkdir -p test-output/cmd/glacial/RubberSheetConflateTest

# Broke this into seperate lines to make reading easier.
hoot conflate -C Testing.conf -D uuid.helper.repeatable=true -D conflate.pre.ops=hoot::RubberSheet -D "rubber.sheet.ref=true" -D "rubber.sheet.minimum.ties=10" -D "unify.optimizer.time.limit=30" test-files/DcGisRoads.osm test-files/DcTigerRoads.osm test-output/cmd/glacial/RubberSheetConflateTest/output.osm

hoot diff test-output/cmd/glacial/RubberSheetConflateTest/output.osm test-files/cmd/glacial/RubberSheetConflateTest/output.osm ||
(echo "You should run: diff test-output/cmd/glacial/RubberSheetConflateTest/output.osm test-files/cmd/glacial/RubberSheetConflateTest/output.osm" ;\
echo "If it looks good then copy a new baseline: cp test-output/cmd/glacial/RubberSheetConflateTest/output.osm test-files/cmd/glacial/RubberSheetConflateTest/output.osm"; exit -1)

