#!/bin/bash
set -e

# Test the GGDMv30 Translation

inputDir=test-files/cmd/slow/GGDMv30
outputDir=test-output/cmd/slow/ggdm30_translation

TRANS=$HOOT_HOME/translations/GGDMv30.js

mkdir -p $outputDir
rm -rf $outputDir/*

# Normal Hoot options
HOOT_OPT="--debug -C Testing.conf"

# Hoot options for debugging the test input and output
# NOTE: This will generate HEAPS of output.
#HOOT_OPT="--debug -D ogr.debug.dumptags=true -D ogr.debug.lookupcolumn=true -D ogr.debug.lookupclash=true -D ogr.debug.dumpvalidate=true"

# Script to compare shapefiles. NOTE: This might not run on Jenkins.
COMPARE_SHAPE=$HOOT_HOME/scripts/util/CompareShapefiles.py

##### Start of initial setup #####
# Un-comment and run these commands when the source OSM file changes.

# Make shapefiles from the OSM file and then update the input shapefiles
# hoot convert-osm2ogr $HOOT_OPT $TRANS $inputDir/GGDMv30.osm $outputDir".shp"
# cp $outputDir/* $inputDir
# rm -f $outputDir/*
##### End of initial setup #####

#  Jam all of the shapefiles into one OSM file
hoot convert $HOOT_OPT -D schema.translation.script=$TRANS $inputDir/*.shp $outputDir/new_GGDMv30.osm # > tmp/GGDM30_to_OSM.txt

# Uncomment this to update the OSM file if you edit the shapefiles or the translation file.
#cp $outputDir/new_GGDMv30.osm $inputDir/GGDMv30.osm

# Compare the new and old OSM files
# NOTE: ZI031 DATASET_S does not have a UUID (UFI) but it gets one on import which screws up the test
hoot diff $HOOT_OPT --ignore-uuid $outputDir/new_GGDMv30.osm $inputDir/GGDMv30.osm # || diff $outputDir/new_GGDMv30.osm $inputDir/GGDMv30.osm

# Make shapefiles from the new OSM file
# NOTE: This assumes that outputDir does not have any shapefiles in it!
hoot convert $HOOT_OPT -C Testing.conf -D schema.translation.script=$TRANS $outputDir/new_GGDMv30.osm $outputDir".shp"  # > tmp/GGDM30_to_GGDM.txt

##### This set of tests: comparing the individual shapefiles works!
# Make individual shapefiles from the new OSM file
# NOTE: These are 1 x FCODE / file and are in a subdirectory!
#hoot convert-osm2ogr $HOOT_OPT -D ogr.thematic.structure=false $TRANS $outputDir/new_GGDMv30.osm $outputDir/"new_files.shp" # > tmp/GGDM30_to_IndGGDM.txt

# Make individual shapefiles from the old OSM file
# NOTE: These are 1 x FCODE / file and are in a subdirectory!
#hoot convert-osm2ogr $HOOT_OPT -D ogr.thematic.structure=false $TRANS $inputDir/GGDMv30.osm $outputDir/"old_files.shp" # > tmp/GGDM30_to_IndGGDM2.txt

# Now look at the individual shapefiles
# for x in $outputDir/old_files/*.shp; do
#    echo $(basename $x) "  Forward"
#    $COMPARE_SHAPE  $x $outputDir/new_files/$(basename $x)
#    echo $(basename $x) "  Backward"
#    $COMPARE_SHAPE  $outputDir/new_files/$(basename $x) $x
#    echo
# done


#####
# More testing needed before enableing this section.
# Jam all of the shapefiles into one OSM file
# hoot convert-ogr2osm $HOOT_OPT $TRANS $outputDir/second_GGDMv30.osm $inputDir/*.shp > tmp/GGDM30_to_second_OSM.txt
#
# echo
# echo "Compare the second with orig"
# hoot diff --ignore-uuid $outputDir/second_GGDMv30.osm $inputDir/GGDMv30.osm || diff $outputDir/second_GGDMv30.osm $inputDir/GGDMv30.osm
# echo
# echo "Compare the second with new"
# hoot diff --ignore-uuid $outputDir/second_GGDMv30.osm $outputDir/new_GGDMv30.osm || diff $outputDir/second_GGDMv30.osm $outputDir/new_GGDMv30.osm
# echo
# echo

# This is commented out until Jenkins has python-gdal support
#
# Now look at the individual shapefiles
# for x in $inputDir/*.shp; do
#    echo $(basename $x) "  Forward"
#    $COMPARE_SHAPE  $x $outputDir/$(basename $x)
#    echo $(basename $x) "  Backward"
#    $COMPARE_SHAPE  $outputDir/$(basename $x) $x
#    echo
# done

