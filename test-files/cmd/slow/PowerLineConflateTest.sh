#!/bin/bash
set -e

mkdir -p $HOOT_HOME/tmp/
mkdir -p test-output/cmd/slow/PowerLineConflateTest

CONFIG="-C Testing.conf"

hoot conflate --warn $CONFIG -C UnifyingAlgorithm.conf -C ReferenceConflation.conf -D uuid.helper.repeatable=true -D match.creators="ScriptMatchCreator,PowerLine.js" -D merger.creators="ScriptMergerCreator" test-files/cmd/slow/PowerLineConflateTest/power-line-1.osm test-files/cmd/slow/PowerLineConflateTest/power-line-2.osm test-output/cmd/slow/PowerLineConflateTest/output.osm
hoot diff $CONFIG test-output/cmd/slow/PowerLineConflateTest/output.osm test-files/cmd/slow/PowerLineConflateTest/output.osm || diff test-output/cmd/slow/PowerLineConflateTest/output.osm test-files/cmd/slow/PowerLineConflateTest/output.osm
