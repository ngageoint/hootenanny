#!/bin/bash
set -e

# Test reading and writing o2s_X files
TRANS_TDS40_IN=$HOOT_HOME/translations/TDSv40.js
TRANS_TDS40_OUT=$HOOT_HOME/translations/TDSv40.js
TRANS_TDS61_IN=$HOOT_HOME/translations/TDSv61_to_OSM.js
TRANS_TDS61_OUT=$HOOT_HOME/translations/TDSv61_to_OGR.js
TRANS_MGCP_IN=$HOOT_HOME/translations/MGCP_TRD4.js
TRANS_MGCP_OUT=$HOOT_HOME/translations/MGCP_TRD4.js
TRANS_GGDM_IN=$HOOT_HOME/translations/GGDMv30.js
TRANS_GGDM_OUT=$HOOT_HOME/translations/GGDMv30.js

inputFile=test-files/o2s_test.osm
tds40File=test-files/o2s_tds40.osm
ggdmFile=test-files/o2s_ggdm.osm

outputDir=test-output/o2s
mkdir -p $outputDir
rm -rf $outputDir/*

# Normal Hoot options
HOOT_OPT="--warn"

# Hoot options for debugging the test input and output
# NOTE: This will generate HEAPS of output.
#HOOT_OPT="--info -D ogr.debug.dumptags=true -D ogr.debug.lookupcolumn=true -D ogr.debug.lookupclash=true -D ogr.debug.dumpvalidate=true"

##### Start Tests #####
# Export files
echo "### TDSv40 ###"
hoot convert $HOOT_OPT -D ogr.thematic.structure=false $inputFile $outputDir/tds40.shp --trans $TRANS_TDS40_OUT
hoot convert --debug $outputDir/tds40/HUT_P.shp $outputDir/tds40/o2s_*.shp $outputDir/tds40/ORCHARD_S.shp $outputDir/tds40/RIVER_C.shp $outputDir/tds40.osm --trans $TRANS_TDS40_IN

hoot diff $outputDir/tds40.osm $tds40File || diff $outputDir/tds40.osm $tds40File

echo
echo "### TDSv61 ###"
hoot convert $HOOT_OPT -D ogr.thematic.structure=false $inputFile $outputDir/tds61.shp --trans $TRANS_TDS61_OUT
hoot convert --debug $outputDir/tds61/HUT_P.shp $outputDir/tds61/o2s_*.shp $outputDir/tds61/ORCHARD_S.shp $outputDir/tds61/RIVER_C.shp $outputDir/tds61.osm --trans $TRANS_TDS61_IN

hoot diff $outputDir/tds61.osm $inputFile || diff $outputDir/tds61.osm $inputFile

echo
echo "### MGCP ###"
hoot convert $HOOT_OPT $inputFile $outputDir/mgcp.shp --trans $TRANS_MGCP_OUT
hoot convert --debug $outputDir/mgcp/PAL099.shp $outputDir/mgcp/o2s_*.shp $outputDir/mgcp/AEA040.shp $outputDir/mgcp/LBH140.shp $outputDir/mgcp.osm --trans $TRANS_MGCP_IN

hoot diff $outputDir/mgcp.osm $inputFile || diff $outputDir/mgcp.osm $inputFile

echo
echo "### GGDM ###"
hoot convert $HOOT_OPT -D ogr.thematic.structure=false $inputFile $outputDir/ggdm.shp --trans $TRANS_GGDM_OUT
hoot convert --debug $outputDir/ggdm/HUT_P.shp $outputDir/ggdm/o2s_*.shp $outputDir/ggdm/ORCHARD_S.shp $outputDir/ggdm/RIVER_C.shp $outputDir/ggdm.osm --trans $TRANS_GGDM_IN

hoot diff $outputDir/ggdm.osm $ggdmFile || diff $outputDir/ggdm.osm $ggdmFile
