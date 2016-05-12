#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/EvalMoveCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
outputdir=$HOOT_HOME/test-output/cmd/slow/EvalMoveCmdTest
myCmd="hoot eval-move 10 1.0,1.0,5.0,5.0 $outputdir"

# Run test, remove stuff that looks like a time string
$myCmd | sed "s/[0-9]\{2\}:[0-9]\{2\}:[0-9]\{2\}.[0-9]\{3\} //g"
