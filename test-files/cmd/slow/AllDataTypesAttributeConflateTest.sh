#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/AllDataTypesAttributeConflateTest
OUT_DIR=test-output/cmd/slow/AllDataTypesAttributeConflateTest
mkdir -p $OUT_DIR

CONFIG="--warn -C Testing.conf"

hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C AttributeConflation.conf -D writer.include.debug.tags=true test-files/conflate/unified/AllDataTypesA.osm test-files/conflate/unified/AllDataTypesB.osm $OUT_DIR/output.osm

hoot diff $CONFIG $IN_DIR/output.osm $OUT_DIR/output.osm || diff $IN_DIR/output.osm $OUT_DIR/output.osm
