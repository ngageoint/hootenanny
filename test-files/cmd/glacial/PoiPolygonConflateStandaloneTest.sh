#!/bin/bash
set -e

inputDir=test-files/cmd/glacial/PoiPolygonConflateStandaloneTest
outputDir=test-output/cmd/glacial/PoiPolygonConflateStandaloneTest

mkdir -p $HOOT_HOME/tmp/
mkdir -p $outputDir

hoot conflate --warn -C UnifyingAlgorithm.conf \
  -C ReferenceConflation.conf \
  -C Testing.conf \
  -D uuid.helper.repeatable=true \
  -D match.creators="hoot::PoiPolygonMatchCreator" \
  -D merger.creators="hoot::PoiPolygonMergerCreator" \
  -D address.scorer.enable.caching=true \
  $inputDir/PoiPolygon1.osm $inputDir/PoiPolygon2.osm $outputDir/output1.osm

hoot diff -C Testing.conf $outputDir/output1.osm $inputDir/output1.osm || diff $outputDir/output1.osm $inputDir/output1.osm
