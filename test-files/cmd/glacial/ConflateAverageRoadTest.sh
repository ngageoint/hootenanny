#!/bin/bash
set -e

IN_DIR=test-files/cmd/glacial/ConflateAverageRoadTest
OUT_DIR=test-output/cmd/glacial/ConflateAverageRoadTest
mkdir -p $OUT_DIR
LOG_LEVEL=--error

hoot conflate $LOG_LEVEL -C AverageConflation.conf -C UnifyingAlgorithm.conf -C Testing.conf -D match.creators="hoot::HighwayMatchCreator" -D merger.creators="hoot::HighwayMergerCreator" test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUT_DIR/output-unifying.osm
hoot diff $LOG_LEVEL $OUT_DIR/output-unifying.osm $IN_DIR/output-unifying.osm || diff $OUT_DIR/output-unifying.osm $IN_DIR/output-unifying.osm

hoot conflate $LOG_LEVEL -C AverageConflation.conf -C NetworkAlgorithm.conf -C Testing.conf -D match.creators="hoot::NetworkMatchCreator" -D merger.creators="hoot::NetworkMergerCreator" test-files/DcGisRoads.osm test-files/DcTigerRoads.osm $OUT_DIR/output-network.osm
hoot diff $LOG_LEVEL $OUT_DIR/output-network.osm $IN_DIR/output-network.osm || diff $OUT_DIR/output-network.osm $IN_DIR/output-network.osm
