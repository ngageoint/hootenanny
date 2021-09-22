#!/bin/bash
set -e

inputDir=test-files/cmd/glacial/PoiPolygonConflateStandaloneTest
checkDir=test-files/cmd/glacial/PoiPolygonConflateCombinedTest
outputDir=test-output/cmd/glacial/PoiPolygonConflateCombinedTest

mkdir -p $HOOT_HOME/tmp/
mkdir -p $outputDir

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

if [ -f "test-output/test-validation-enabled" ]; then
  hoot validate $LOG_LEVEL $CONFIG $outputDir/output.osm \
    --report-output $outputDir/output-validation-report --output $outputDir/output-validated.osm
  diff $checkDir/output-validation-report $outputDir/output-validation-report
fi
