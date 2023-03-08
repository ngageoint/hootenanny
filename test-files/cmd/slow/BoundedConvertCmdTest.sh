#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/slow/BoundedConvertCmdTest
OUTPUT_DIR=test-output/cmd/slow/BoundedConvertCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

TRANSLATION="-D schema.translation.script=translations"
TRANSLATION_TDS="$TRANSLATION/TDSv70.js"

echo ""
echo "Bounded OSM to SHP - Polygons"
echo ""
rm -rf $OUTPUT_DIR/clipped_poly/
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_TDS \
  -D bounds="-104.80643134164,39.59123123677,-104.8042456804,39.59369827561" \
  -D writer.crop.features.crossing.bounds=true \
  test-files/ToyBuildingsTestA.osm $OUTPUT_DIR/clipped_poly.shp
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_TDS \
  $OUTPUT_DIR/clipped_poly/ $OUTPUT_DIR/clipped_poly.osm
hoot diff $LOG_LEVEL $CONFIG --ignore-uuid $INPUT_DIR/clipped_poly.osm $OUTPUT_DIR/clipped_poly.osm

echo ""
echo "Bounded OSM to SHP - Linestrings"
echo ""
rm -rf $OUTPUT_DIR/clipped_lines/
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_TDS \
  -D bounds="-104.90022388323,38.85350714489,-104.8983212846,38.85504576826" \
  -D writer.crop.features.crossing.bounds=true \
  test-files/ToyTestA.osm $OUTPUT_DIR/clipped_lines.shp
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_TDS \
  $OUTPUT_DIR/clipped_lines/ $OUTPUT_DIR/clipped_lines.osm
hoot diff $LOG_LEVEL $CONFIG --ignore-uuid $INPUT_DIR/clipped_lines.osm $OUTPUT_DIR/clipped_lines.osm

LOG_LEVEL="--error"

echo ""
echo "Bounded OSM to GeoJSON - Polygons and Linestrings"
echo ""
hoot convert $LOG_LEVEL $CONFIG \
  -D bounds="-71.46477676157,42.4860359843,-71.45903473335,42.49004008401" \
  -D writer.crop.features.crossing.bounds=true \
  test-files/BostonSubsetRoadBuilding_FromOsm.osm $OUTPUT_DIR/clipped_boston.geojson
hoot diff $LOG_LEVEL $CONFIG --ignore-uuid $INPUT_DIR/clipped_boston.geojson $OUTPUT_DIR/clipped_boston.geojson

echo ""
echo "Bounded and translated OSM to GeoJSON - Polygons and Linestrings"
echo ""
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_TDS \
  -D bounds="-71.46477676157,42.4860359843,-71.45903473335,42.49004008401" \
  -D writer.crop.features.crossing.bounds=true \
  test-files/BostonSubsetRoadBuilding_FromOsm.osm $OUTPUT_DIR/clipped_boston_translate.geojson
hoot diff $LOG_LEVEL $CONFIG --ignore-uuid $INPUT_DIR/clipped_boston_translate.geojson $OUTPUT_DIR/clipped_boston_translate.geojson
