#!/bin/bash
set -e

# Test to make sure gdal2tiles.py works

INPUT_DIR=test-files/gdal2tiles
OUTPUT_DIR=test-output/gdal2tiles

INPUT_FILE=$INPUT_DIR/RomanColosseum_WV2naturalcolor_clip.tif
OUTPUT_TILES=$OUTPUT_DIR/new_tiles

REFERENCE_TILES=$INPUT_DIR/good_tiles.zip

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
unzip -q -d $OUTPUT_DIR $INPUT_DIR/good_tiles.zip

OLDFILES=`ls -R $OUTPUT_DIR/good_tiles | wc -l`
NEWFILES=`ls -R $OUTPUT_TILES | wc -l`

if [ "$NEWFILES" != "$OLDFILES" ] ; then
    echo "gdal2tiles.py may not have run correctly. Expecting" $OLDFILES "tiles. Got" $NEWFILES
    #exit 1
fi


OLDSIZE=`du -c $OUTPUT_DIR/good_tiles | grep total  | awk '{print $1}'`
NEWSIZE=`du -c $OUTPUT_TILES | grep total | awk '{print $1}'`

if [ "$NEWFILES" != "$OLDFILES" ] ; then
    echo "gdal2tiles.py may not have run correctly. Expecting" $OLDSIZE "tile size. Got" $NEWSIZE
    exit 1
fi

