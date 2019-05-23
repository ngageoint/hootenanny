#!/bin/bash
set -e

mkdir -p test-output/cmd/slow/translation/

function compareFiles {
    hoot diff -C Testing.conf $1 $2 || diff $1 $2
}

hoot convert -C Testing.conf -D shape.file.writer.cols="LTN;WTC;HCT;UID" test-files/translation/MgcpTest.osm test-output/cmd/slow/translation/MgcpTest.shp
hoot convert -C Testing.conf --warn -D schema.translation.script=translations/MgcpTest.js test-output/cmd/slow/translation/MgcpTestLines.shp test-output/cmd/slow/translation/MgcpTranslated.osm

# Un-comment this to update the OSM file
#cp test-output/cmd/slow/translation/MgcpTranslated.osm test-files/translation/MgcpTranslated.osm

compareFiles test-output/cmd/slow/translation/MgcpTranslated.osm test-files/translation/MgcpTranslated.osm

#
# Added commands to build the MGCPv3 OSM test files
#
# Mattj Oct 13

function checkMgcpV3 {
    OUT=test-output/cmd/slow/translation/Mgcpv3_$1.osm
    IN=test-files/MGCPv3/$1.shp
    hoot convert -C Testing.conf --warn -D schema.translation.script=translations/MgcpTest.js $IN $OUT
    #echo $IN  $OUT
    compareFiles $OUT test-files/MGCPv3/$1-output.osm
    # Uncomment this to update what we compare with.
    #cp $OUT test-files/MGCPv3/$1-output.osm
    # Export as TRDv4
    OUT4=test-output/cmd/slow/translation/$1
    rm -rf $OUT4
    hoot convert -C Testing.conf --debug -D schema.translation.script=translations/MGCP_TRD4.js $OUT $OUT4.shp
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

