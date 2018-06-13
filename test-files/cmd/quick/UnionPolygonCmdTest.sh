#!/bin/bash
#set -e


#DEBUG_OPT="--trace"


INPUTFILEDIR=$HOOT_HOME/test-files/cmd/quick/UnionPolygonCmdTest
OUTPUTFILEDIR=$HOOT_HOME/test-output/cmd/quick/UnionPolygonCmdTest

mkdir -p $OUTPUTFILEDIR

# Commented out tests to keep this "quick"

# Input: 2 x relation polygons
# Output: 1 x polygon 
# INPUTFILE1=$INPUTFILEDIR/square_relation.osm
# INPUTFILE2=$INPUTFILEDIR/diamond_relation.osm
# OUTPUTFILE=$OUTPUTFILEDIR/union_relation.osm
# COMPAREFILE=$INPUTFILEDIR/union_polygon.osm

# hoot union-polygons $OUTPUTFILE $INPUTFILE1 $INPUTFILE2
# hoot map-diff $COMPAREFILE $OUTPUTFILE || diff $COMPAREFILE $OUTPUTFILE

# Input: 2 x polygons
# Output: 1 x polygon 
INPUTFILE1=$INPUTFILEDIR/square_polygon.osm
INPUTFILE2=$INPUTFILEDIR/diamond_polygon.osm
OUTPUTFILE=$OUTPUTFILEDIR/union_polygon.osm
COMPAREFILE=$INPUTFILEDIR/union_polygon.osm

hoot union-polygons $DEBUG_OPT $OUTPUTFILE $INPUTFILE1 $INPUTFILE2
hoot map-diff $COMPAREFILE $OUTPUTFILE || diff $COMPAREFILE $OUTPUTFILE


# Input: 1 x multipolygon, 1 x polygon
# Output: 1 x polygon 
# INPUTFILE1=$INPUTFILEDIR/multi_polygon1.osm
# INPUTFILE2=$INPUTFILEDIR/diamond_polygon.osm
# OUTPUTFILE=$OUTPUTFILEDIR/union_multi_one.osm
# COMPAREFILE=$INPUTFILEDIR/union_multi_one.osm

# hoot union-polygons $DEBUG_OPT $OUTPUTFILE $INPUTFILE1 $INPUTFILE2
# hoot map-diff $COMPAREFILE $OUTPUTFILE || diff $COMPAREFILE $OUTPUTFILE


# Input: 2 x multipolygons
# Output: 1 x polygon 
INPUTFILE1=$INPUTFILEDIR/multi_polygon1.osm
INPUTFILE2=$INPUTFILEDIR/multi_polygon2.osm
OUTPUTFILE=$OUTPUTFILEDIR/union_multi_both.osm
COMPAREFILE=$INPUTFILEDIR/union_multi_both.osm

hoot union-polygons $DEBUG_OPT $OUTPUTFILE $INPUTFILE1 $INPUTFILE2
hoot map-diff $COMPAREFILE $OUTPUTFILE || diff $COMPAREFILE $OUTPUTFILE
