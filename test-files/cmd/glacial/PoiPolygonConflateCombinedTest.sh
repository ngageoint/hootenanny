#!/bin/bash
set -e

inputDir=test-files/cmd/glacial/PoiPolygonConflateStandaloneTest

checkDir=test-files/cmd/glacial/PoiPolygonConflateCombinedTest

outputDir=test-output/cmd/glacial/PoiPolygonConflateCombinedTest


mkdir -p $HOOT_HOME/tmp/
mkdir -p $outputDir

# Making this info instead of warning output to catch a situation where a generic script was calculating the search radius multiple times.
hoot conflate -C UnifyingAlgorithm.conf \
  -C ReferenceConflation.conf \
  -C Testing.conf \
  -D uuid.helper.repeatable=true \
  -D address.scorer.enable.caching=true \
  $inputDir/PoiPolygon1.osm \
  $inputDir/PoiPolygon2.osm \
  $outputDir/output1.osm

hoot diff -C Testing.conf $outputDir/output1.osm $checkDir/output1.osm || diff $outputDir/output1.osm $checkDir/output1.osm
