#!/bin/bash
set -e

inputDir=test-files/cmd/glacial/PoiPolygonConflateStandaloneTest
checkDir=test-files/cmd/glacial/PoiPolygonConflateCombinedTest
outputDir=test-output/cmd/glacial/PoiPolygonConflateCombinedTest

mkdir -p $HOOT_HOME/tmp/
mkdir -p $outputDir

source scripts/core/ScriptTestUtils.sh

# Making this info instead of warning output to catch a situation where a generic script was 
# calculating the search radius multiple times.
hoot conflate -C UnifyingAlgorithm.conf \
  -C ReferenceConflation.conf \
  -C Testing.conf \
  -D match.creators="BuildingMatchCreator;PoiPolygonMatchCreator;ScriptMatchCreator,Poi.js" \
  -D merger.creators="BuildingMergerCreator;PoiPolygonMergerCreator;ScriptMergerCreator" \
  -D uuid.helper.repeatable=true \
  -D address.scorer.enable.caching=true \
  $inputDir/PoiPolygon1.osm \
  $inputDir/PoiPolygon2.osm \
  $outputDir/output.osm
hoot diff -C Testing.conf $outputDir/output.osm $checkDir/output.osm || diff $outputDir/output.osm $checkDir/output.osm

validateTestOutput $outputDir/output.osm $outputDir/output-validation-report \
  $outputDir/output-validated.osm $checkDir/output-validation-report
