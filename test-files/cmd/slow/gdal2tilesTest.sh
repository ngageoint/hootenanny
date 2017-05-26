#!/bin/bash
set -e

# Test to make sure gdal2tiles.py works

INPUT_DIR=test-files/gdal2tiles
OUTPUT_DIR=test-output/gdal2tiles

INPUT_FILE=$INPUT_DIR/RomanColosseum_WV2naturalcolor_clip.tif
OUTPUT_TILES=$OUTPUT_DIR/new_tiles

OPT="-w none -z 0-20 -t new_tiles"

mkdir -p $OUTPUT_DIR
rm -rf $OUTPUT_DIR/*

mkdir $OUTPUT_TILES

# 1. Make sure gdal2tiles.py can run
if ! which gdal2tiles.py > /dev/null; then
    echo "Cant find gdal2tiles.py"
    exit 1
fi


# 2. Make some tiles
gdal2tiles.py $OPT $INPUT_FILE $OUTPUT_TILES


# 3. Compare the tiles to a known good set of tiles.
NEWFILES=`ls -R $OUTPUT_TILES | wc -l`

# The expected number of files in the directory = 303
[ $NEWFILES != 303 ] && echo "gdal2tiles.py: Expecting 303 files Got" $NEWFILES

# The expected size of the new tiles is 2700
NEWSIZE=`du -c $OUTPUT_TILES | grep total | awk '{print $1}'`

[ $NEWSIZE -gt 2710 ] && echo "gdal2tiles.py: Expecting a tile size around 2700. Got" $NEWSIZE

[ $NEWSIZE -lt 2450 ] && echo "gdal2tiles.py: Expecting a tile size between 2500 and 2700. Got" $NEWSIZE


