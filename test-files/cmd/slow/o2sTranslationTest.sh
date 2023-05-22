#!/bin/bash
set -e

# Test reading and writing o2s_X files
TRANS_TDS40=$HOOT_HOME/translations/TDSv40.js
TRANS_TDS61=$HOOT_HOME/translations/TDSv61.js
TRANS_MGCP=$HOOT_HOME/translations/MGCP_TRD4.js
TRANS_GGDM=$HOOT_HOME/translations/GGDMv30.js

inputFile=test-files/o2s_test.osm
tds40File=test-files/o2s_tds40.osm
ggdmFile=test-files/o2s_ggdm.osm

outputDir=test-output/cmd/slow/o2s
mkdir -p $outputDir
rm -rf $outputDir/*

# Normal Hoot options
HOOT_OPT="--warn -C Testing.conf"

# Hoot options for debugging the test input and output
# NOTE: This will generate HEAPS of output.
# HOOT_OPT="--info -D ogr.debug.dumptags=true -D ogr.debug.lookupcolumn=true -D ogr.debug.lookupclash=true -D ogr.debug.dumpvalidate=true"

##### Start Tests #####
# Export files
echo "### TDSv40 ###"
hoot convert $HOOT_OPT -D schema.translation.script=$TRANS_TDS40 -D writer.thematic.structure=false $inputFile $outputDir/tds40.shp
hoot convert --debug -C Testing.conf -D schema.translation.script=$TRANS_TDS40 $outputDir/tds40/HUT_P.shp $outputDir/tds40/o2s_*.shp $outputDir/tds40/ORCHARD_S.shp $outputDir/tds40/RIVER_C.shp $outputDir/tds40.osm

hoot diff -C Testing.conf $outputDir/tds40.osm $tds40File || diff $outputDir/tds40.osm $tds40File

echo
echo "### TDSv61 ###"
hoot convert $HOOT_OPT -D schema.translation.script=$TRANS_TDS61 -D writer.thematic.structure=false $inputFile $outputDir/tds61.shp
hoot convert --debug -C Testing.conf -D schema.translation.script=$TRANS_TDS61 $outputDir/tds61/HUT_P.shp $outputDir/tds61/o2s_*.shp $outputDir/tds61/ORCHARD_S.shp $outputDir/tds61/RIVER_C.shp $outputDir/tds61.osm

hoot diff -C Testing.conf $outputDir/tds61.osm $inputFile || diff $outputDir/tds61.osm $inputFile

echo
echo "### MGCP ###"
hoot convert $HOOT_OPT -D schema.translation.script=$TRANS_MGCP -D writer.thematic.structure=false $inputFile $outputDir/mgcp.shp
hoot convert --debug -C Testing.conf -D schema.translation.script=$TRANS_MGCP $outputDir/mgcp/PAL099.shp $outputDir/mgcp/o2s_*.shp $outputDir/mgcp/AEA040.shp $outputDir/mgcp/LBH140.shp $outputDir/mgcp.osm

hoot diff -C Testing.conf $outputDir/mgcp.osm $inputFile || diff $outputDir/mgcp.osm $inputFile

echo
echo "### GGDM ###"
hoot convert $HOOT_OPT -D schema.translation.script=$TRANS_GGDM -D writer.thematic.structure=false $inputFile $outputDir/ggdm.shp
hoot convert --debug -C Testing.conf -D schema.translation.script=$TRANS_GGDM $outputDir/ggdm/HUT_P.shp $outputDir/ggdm/o2s_*.shp $outputDir/ggdm/ORCHARD_S.shp $outputDir/ggdm/RIVER_C.shp $outputDir/ggdm.osm

hoot diff -C Testing.conf $outputDir/ggdm.osm $ggdmFile || diff $outputDir/ggdm.osm $ggdmFile
