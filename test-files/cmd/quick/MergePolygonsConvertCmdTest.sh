#!/bin/bash
#set -e

DEBUG_OPT="--warn -C Testing.conf"

INPUTFILEDIR=test-files/cmd/quick/MergePolygonsConvertCmdTest
OUTPUTFILEDIR=test-output/cmd/quick/MergePolygonsConvertCmdTest

mkdir -p $OUTPUTFILEDIR


# Input: 2 x polygons
# Output: 1 x polygon 
INPUTFILE1=$INPUTFILEDIR/square_polygon.osm
INPUTFILE2=$INPUTFILEDIR/diamond_polygon.osm
OUTPUTFILE=$OUTPUTFILEDIR/union_polygon.osm
COMPAREFILE=$INPUTFILEDIR/union_polygon.osm

hoot convert $DEBUG_OPT -D convert.ops="hoot::UnionPolygonsOp" $INPUTFILE1 $INPUTFILE2 $OUTPUTFILE
hoot diff $DEBUG_OPT $COMPAREFILE $OUTPUTFILE || diff $COMPAREFILE $OUTPUTFILE

# Input: 2 x multipolygons
# Output: 1 x polygon 
INPUTFILE1=$INPUTFILEDIR/multi_polygon1.osm
INPUTFILE2=$INPUTFILEDIR/multi_polygon2.osm
OUTPUTFILE=$OUTPUTFILEDIR/union_multi_both.osm
COMPAREFILE=$INPUTFILEDIR/union_multi_both.osm

hoot convert $DEBUG_OPT -D convert.ops="hoot::UnionPolygonsOp" $INPUTFILE1 $INPUTFILE2 $OUTPUTFILE
hoot diff $DEBUG_OPT $COMPAREFILE $OUTPUTFILE || diff $COMPAREFILE $OUTPUTFILE
