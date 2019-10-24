#!/bin/bash
set -e

IN_DIR=test-files/conflate/unified
IN_DIR_2=test-files/cmd/quick/ConflateTagFilterTest
OUT_DIR=test-output/cmd/quick/ConflateTagFilterTest
mkdir -p $OUT_DIR

HOOT_OPTS="--warn -D uuid.helper.repeatable=true -C Testing.conf"

hoot conflate $HOOT_OPTS \
 -C UnifyingAlgorithm.conf -C ReferenceConflation.conf \
 -D conflate.tag.filter="{ \"must\": [ { \"tag\": \"name=starbucks\" } ] }" \
 $IN_DIR/AllDataTypesA.osm $IN_DIR/AllDataTypesB.osm $OUT_DIR/out.osm

hoot diff $HOOT_OPTS $OUT_DIR/out.osm $IN_DIR_2/out.osm


