#!/bin/bash
set -e

inputDir=test-files/cmd/glacial/PoiPolygonConflateStandaloneTest
outputDir=test-output/cmd/glacial/PoiPolygonConflateStandaloneTest

mkdir -p $HOOT_HOME/tmp/
mkdir -p $outputDir

TO_VALIDATE_1=test-output/cmd/glacial/PoiPolygonConflateStandaloneTest/output.osm
VALIDATION_REPORT_GOLD_1=test-files/cmd/glacial/PoiPolygonConflateStandaloneTest/output-validation-report
hoot conflate --warn -C UnifyingAlgorithm.conf \
  -C ReferenceConflation.conf \
  -C Testing.conf \
  -D uuid.helper.repeatable=true \
  -D match.creators="PoiPolygonMatchCreator" \
  -D merger.creators="PoiPolygonMergerCreator" \
  -D address.scorer.enable.caching=true \
  $inputDir/PoiPolygon1.osm $inputDir/PoiPolygon2.osm $outputDir/output.osm

hoot diff -C Testing.conf $outputDir/output.osm $inputDir/output.osm || diff $outputDir/output1.osm $inputDir/output.osm
