#!/bin/bash
set -e

mkdir -p test-output/translation/

function compareFiles {
    hoot --is-match $1 $2 || diff $1 $2
}

hoot --osm2shp "LTN,WTC,HCT,UID" test-files/translation/MgcpTest.osm test-output/translation/MgcpTest.shp
hoot --ogr2osm translations/MgcpTest.js test-output/translation/MgcpTranslated.osm test-output/translation/MgcpTestLines.shp

compareFiles test-output/translation/MgcpTranslated.osm test-files/translation/MgcpTranslated.osm

#
# Added commands to build the MGCPv3 OSM test files
#
# Mattj Oct 13

function checkMgcpV3 {
    OUT=test-output/translation/Mgcpv3_$1.osm
    IN=test-files/MGCPv3/$1.shp
    hoot --ogr2osm translations/MgcpTest.js $OUT $IN
    #echo $IN  $OUT
    compareFiles $OUT test-files/MGCPv3/$1-output.osm
    # Uncomment this to update what we compare with.
    #cp $OUT test-files/MGCPv3/$1-output.osm
    # Export as TRDv4
    OUT4=test-output/translation/$1
    rm -rf $OUT4
    hoot --osm2ogr translations/MGCP_TRD4.js $OUT $OUT4.shp
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

