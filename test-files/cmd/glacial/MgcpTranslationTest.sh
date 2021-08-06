#!/bin/bash
set -e

mkdir -p test-output/cmd/glacial/translation/

function compareFiles {
    hoot diff --error-limit 20 $1 $2 || diff $1 $2
}

CONFIG="-C Testing.conf"

hoot convert $CONFIG -D shape.file.writer.cols="LTN;WTC;HCT;UID" test-files/translation/MgcpTest.osm test-output/cmd/glacial/translation/MgcpTest.shp
hoot convert --warn $CONFIG -D schema.translation.script=translations/MgcpTest.js test-output/cmd/glacial/translation/MgcpTestLines.shp test-output/cmd/glacial/translation/MgcpTranslated.osm

# Un-comment this to update the OSM file
#cp test-output/cmd/glacial/translation/MgcpTranslated.osm test-files/translation/MgcpTranslated.osm

compareFiles test-output/cmd/glacial/translation/MgcpTranslated.osm test-files/translation/MgcpTranslated.osm

# LOTS of debug output
# CONFIG="--debug -C Testing.conf -D ogr.debug.dumptags=true -D ogr.debug.lookupcolumn=true -D ogr.debug.lookupclash=true -D ogr.debug.dumpvalidate=true"

function checkMgcpV3 {
    OUT=test-output/cmd/glacial/translation/Mgcpv3_$1.osm
    IN=test-files/MGCPv3/$1.shp
    hoot convert --warn $CONFIG -D schema.translation.script=translations/MgcpTest.js $IN $OUT
    #echo $IN  $OUT
    # Uncomment this to update what we compare with.
    # cp $OUT test-files/MGCPv3/$1-outputX.osm
    compareFiles $OUT test-files/MGCPv3/$1-output.osm
    # Export as TRDv4
    OUT4=test-output/cmd/glacial/translation/$1
    rm -rf $OUT4
    hoot convert --info $CONFIG -D schema.translation.script=translations/MGCP_TRD4.js $OUT $OUT4.shp
}

# Building Areas
checkMgcpV3 AAL015
#  BUA
checkMgcpV3 AAL020
# Railways
checkMgcpV3 LAN010
# Cart Track
checkMgcpV3 LAP010
# Roads
checkMgcpV3 LAP030
# Trails
checkMgcpV3 LAP050
# Rivers
checkMgcpV3 LBH140
# Building Points
checkMgcpV3 PAL015
# Bridge Lines
checkMgcpV3 LAQ040
# Tree Areas
checkMgcpV3 AEC030
# Scrub Areas
checkMgcpV3 AEB020
#  Storage Tank Areas
checkMgcpV3 AAM070
#  Storage Tank Points
checkMgcpV3 PAM070

