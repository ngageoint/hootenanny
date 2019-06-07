#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/glacial/PowerLineConflateTest

hoot conflate -C Testing.conf --warn -D uuid.helper.repeatable=true -D match.creators="hoot::ScriptMatchCreator,PowerLine.js" -D merger.creators="hoot::ScriptMergerCreator" test-files/cmd/glacial/PowerLineConflateTest/power-line-1.osm test-files/cmd/glacial/PowerLineConflateTest/power-line-2.osm test-output/cmd/glacial/PowerLineConflateTest/output.osm
hoot diff test-output/cmd/glacial/PowerLineConflateTest/output.osm test-files/cmd/glacial/PowerLineConflateTest/output.osm || diff test-output/cmd/glacial/PowerLineConflateTest/output.osm test-files/cmd/glacial/PowerLineConflateTest/output.osm
