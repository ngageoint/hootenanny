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
  -D match.creators="PoiPolygonMatchCreator" \
  -D merger.creators="PoiPolygonMergerCreator" \
  -D address.scorer.enable.caching=true \
  $inputDir/PoiPolygon1.osm $inputDir/PoiPolygon2.osm $outputDir/output.osm
hoot diff -C Testing.conf $outputDir/output.osm $inputDir/output.osm || diff $outputDir/output1.osm $inputDir/output.osm

if [ -f "test-output/test-validation-enabled" ]; then
  hoot validate $LOG_LEVEL $CONFIG $outputDir/output.osm \
    --report-output $outputDir/output-validation-report --output $outputDir/output-validated.osm
  diff $inputDir/output-validation-report $outputDir/output-validation-report
fi
