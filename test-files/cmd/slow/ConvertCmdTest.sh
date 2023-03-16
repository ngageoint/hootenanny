#!/bin/bash
set -e

INPUT_DIR=test-files/cmd/slow/ConvertCmdTest
OUTPUT_DIR=test-output/cmd/slow/ConvertCmdTest
rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR

RECURSIVE_INPUT=test-files/cmd/slow/CountCmdTest/inputDir

SEPARATE_OUTPUT_INPUT_1=$OUTPUT_DIR/ToyTestA.shp
SEPARATE_OUTPUT_INPUT_2=$OUTPUT_DIR/ToyTestB.osm.pbf
SEPARATE_OUTPUT_OUTPUT_1=$OUTPUT_DIR/ToyTestA-converted.osm
SEPARATE_OUTPUT_OUTPUT_2=$OUTPUT_DIR/ToyTestB-converted.osm
GOLD_FILE_SEPARATE_OUTPUT_1=$OUTPUT_DIR/ToyTestAShp.osm
GOLD_FILE_SEPARATE_OUTPUT_2=test-files/ToyTestB.osm.pbf

GOLD_FILE_CONTAINER_OUTPUT=$INPUT_DIR/combined_shapefile.osm

CONFIG="-C Testing.conf"
LOG_LEVEL="--warn"

TRANSLATION="-D schema.translation.script=translations"
TRANSLATION_TDR4="$TRANSLATION/MGCP_TRD4.js"
TRANSLATION_GGDM="$TRANSLATION/GGDMv30.js"
TRANSLATION_MGCP="$TRANSLATION/MgcpTest.js"
TRANSLATION_DNC="$TRANSLATION/DNC.js"

echo ""
echo "Multiple OSM to single OSM..."
echo ""
hoot convert $LOG_LEVEL $CONFIG test-files/ToyTestA.osm test-files/ToyTestB.osm $OUTPUT_DIR/ToyTestC.osm
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/ToyTestC.osm $OUTPUT_DIR/ToyTestC.osm

echo ""
echo "Dir with no filter to single OSM..."
echo ""
# The input dir has duplicated files. Set to not load data source IDs to load in the duplicated 
# elements. The output should have duplicated features in it.
hoot convert $LOG_LEVEL $CONFIG -D non.osm.convert.merge.nearby.nodes=false -D reader.use.data.source.ids=false $RECURSIVE_INPUT $OUTPUT_DIR/recursive-out-1.osm --recursive "*"
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/recursive-out-1.osm $OUTPUT_DIR/recursive-out-1.osm

echo ""
echo "Dir with filter to single OSM..."
echo ""
# In this case there's only one json file in the dir, so the output won't have any duplicated
# elements due to the input filtering.
hoot convert $LOG_LEVEL $CONFIG -D reader.use.data.source.ids=false $RECURSIVE_INPUT $OUTPUT_DIR/recursive-out-2.osm --recursive "*.json"
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/recursive-out-2.osm $OUTPUT_DIR/recursive-out-2.osm

echo ""
echo "PBF to OSM..."
echo ""
hoot convert $LOG_LEVEL $CONFIG test-files/ToyTestA.osm.pbf $OUTPUT_DIR/ToyTestA.osm
cat $OUTPUT_DIR/ToyTestA.osm

echo ""
echo "PBF to SHP..."
echo ""
hoot convert $LOG_LEVEL $CONFIG test-files/ToyTestA.osm.pbf $OUTPUT_DIR/ToyTestA.shp

echo ""
echo "SHP to OSM..."
echo ""
# comparing shapefiles is kinda hard. We'll convert it to a .osm file first.
hoot convert $LOG_LEVEL $CONFIG $OUTPUT_DIR/ToyTestA.shp $OUTPUT_DIR/ToyTestAShp.osm
hoot diff $LOG_LEVEL $CONFIG $OUTPUT_DIR/ToyTestAShp.osm $OUTPUT_DIR/ToyTestAShp.osm

echo ""
echo "Streaming XML read and write..."
echo ""
hoot convert $LOG_LEVEL $CONFIG -D writer.xml.sort.by.id="false" test-files/ToyTestA.osm $OUTPUT_DIR/ToyTestA2.osm
hoot diff $LOG_LEVEL $CONFIG test-files/ToyTestA.osm $OUTPUT_DIR/ToyTestA2.osm

echo ""
echo "POI Criterion..."
echo ""
# test streaming filter output to pois only
hoot convert $LOG_LEVEL $CONFIG -D writer.xml.sort.by.id="false" -D convert.ops="PoiCriterion" test-files/conflate/unified/AllDataTypesA.osm.pbf $OUTPUT_DIR/AllDataTypesA.osm
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/AllDataTypesAPois.osm $OUTPUT_DIR/AllDataTypesA.osm

echo ""
echo "Check for invalid characters. You should see 3 sets of warnings."
echo ""
hoot convert $LOG_LEVEL $CONFIG test-files/io/InvalidCharacters.osm $OUTPUT_DIR/InvalidCharacters.osm

echo ""
echo "Translation check..."
echo ""
# This wasn't working at one point.
hoot convert $LOG_LEVEL $CONFIG -D convert.ops=SchemaTranslationVisitor -D schema.translation.direction=toogr $TRANSLATION_TDR4 test-files/ToyTestA.osm $OUTPUT_DIR/translation-check-out.gdb

echo ""
echo "Multi-layer GDB to OSM..."
echo ""
hoot convert $LOG_LEVEL $CONFIG test-files/cmd/slow/CountCmdTest/input.gdb $OUTPUT_DIR/multi-layer-gdb-out.osm
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/multi-layer-gdb-out.osm $OUTPUT_DIR/multi-layer-gdb-out.osm

echo ""
echo "Multi-layer GPKG to OSM..."
echo ""
hoot convert $LOG_LEVEL $CONFIG test-files/BostonSubsetRoadBuilding_FromOsm.gpkg $OUTPUT_DIR/multi-layer-gpkg-out.osm
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/multi-layer-gpkg-out.osm $OUTPUT_DIR/multi-layer-gpkg-out.osm

echo ""
echo "Multi-layer GDB to OSM single layer only..."
echo ""
hoot convert $LOG_LEVEL $CONFIG "test-files/cmd/slow/CountCmdTest/input.gdb;LAP030" \
  $OUTPUT_DIR/multi-layer-gdb-single-layer-out.osm
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/multi-layer-gdb-single-layer-out.osm $OUTPUT_DIR/multi-layer-gdb-single-layer-out.osm

echo ""
echo "Multiple formats to OSM and writing to separate outputs..."
echo ""
# Copy the inputs to the output directory and work from there, so we don't write to the input dir 
# under source control. Note the file comparison here depends on the output of a previous command in 
# this test and that input doesn't need to be copied to the output dir.
cp $GOLD_FILE_SEPARATE_OUTPUT_2 $OUTPUT_DIR
hoot convert $LOG_LEVEL $CONFIG $SEPARATE_OUTPUT_INPUT_1 $SEPARATE_OUTPUT_INPUT_2 "osm" --separate-output
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_SEPARATE_OUTPUT_1 $SEPARATE_OUTPUT_OUTPUT_1
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_SEPARATE_OUTPUT_2 $SEPARATE_OUTPUT_OUTPUT_2

# TODO: This has proven unstable some of the time both locally and on Jenkins...not sure why yet.
# Just ensure that no errors show up in the command output.
echo ""
echo "OSM to GPKG..."
echo ""
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_GGDM \
  test-files/ToyTestA.osm $OUTPUT_DIR/ToyTestA.gpkg
# Do it again to make sure we can overwrite an existing layer.
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_GGDM \
  test-files/ToyTestA.osm $OUTPUT_DIR/ToyTestA.gpkg
#hoot diff $LOG_LEVEL $CONFIG -D map.comparator.ignore.tag.keys="UFI" \
#  $INPUT_DIR/ToyTestA.gpkg $OUTPUT_DIR/ToyTestA.gpkg

echo ""
echo "ZIP of SHP to OSM..."
echo ""
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_MGCP \
  test-files/MGCPv3.zip $OUTPUT_DIR/zip_convert.osm
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_CONTAINER_OUTPUT $OUTPUT_DIR/zip_convert.osm

echo ""
echo "TAR of SHP to OSM..."
echo ""
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_MGCP \
  test-files/MGCPv3.tar $OUTPUT_DIR/tar_convert.osm
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_CONTAINER_OUTPUT $OUTPUT_DIR/tar_convert.osm

echo ""
echo "TAR GZ of SHP to OSM..."
echo ""
hoot convert $LOG_LEVEL $CONFIG $TRANSLATION_MGCP \
  test-files/MGCPv3.tar.gz $OUTPUT_DIR/tar_gz_convert.osm
hoot diff $LOG_LEVEL $CONFIG $GOLD_FILE_CONTAINER_OUTPUT $OUTPUT_DIR/tar_gz_convert.osm

echo ""
echo "DNC to OSM..."
echo ""
rm -rf $OUTPUT_DIR/DNC17
unzip -q $INPUT_DIR/coa17d.zip -d $OUTPUT_DIR/
hoot convert $LOG_LEVEL $CONFIG -D ogr.add.uuid=false -D reader.add.source.datetime=false $TRANSLATION_DNC \
  gltp:/vrf/$HOOT_HOME/$OUTPUT_DIR/DNC17/COA17D $OUTPUT_DIR/dnc_test.osm
# NOTE: The OGDI driver doesn't produce output in the exact same way every time so don't compare
# the outputs just ensure that no errors show up in the command output
#hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/dnc_test.osm $OUTPUT_DIR/dnc_test.osm

echo ""
echo "Streaming XML read and GeoJSON write..."
echo ""
hoot convert $CONFIG test-files/ToyTestA.osm $OUTPUT_DIR/ToyTestA.geojson
hoot diff $LOG_LEVEL $CONFIG $INPUT_DIR/ToyTestA.geojson $OUTPUT_DIR/ToyTestA.geojson
