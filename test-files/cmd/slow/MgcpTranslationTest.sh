#!/bin/bash
set -e

mkdir -p test-output/translation/

function compareFiles {
    hoot map-diff $1 $2 || diff $1 $2
}

hoot convert test-files/translation/MgcpTest.osm test-output/translation/MgcpTest.shp --cols "LTN,WTC,HCT,UID"
hoot convert --warn test-output/translation/MgcpTestLines.shp test-output/translation/MgcpTranslated.osm --trans translations/MgcpTest.js

# Un-comment this to update the OSM file
#cp test-output/translation/MgcpTranslated.osm test-files/translation/MgcpTranslated.osm

compareFiles test-output/translation/MgcpTranslated.osm test-files/translation/MgcpTranslated.osm

#
# Added commands to build the MGCPv3 OSM test files
#
# Mattj Oct 13

function checkMgcpV3 {
    OUT=test-output/translation/Mgcpv3_$1.osm
    IN=test-files/MGCPv3/$1.shp
    hoot convert --warn $IN $OUT --trans translations/MgcpTest.js
    #echo $IN  $OUT
    compareFiles $OUT test-files/MGCPv3/$1-output.osm
    # Uncomment this to update what we compare with.
    #cp $OUT test-files/MGCPv3/$1-output.osm
    # Export as TRDv4
    OUT4=test-output/translation/$1
    rm -rf $OUT4
    hoot convert --debug $OUT $OUT4.shp --trans translations/MGCP_TRD4.js
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

