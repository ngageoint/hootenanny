#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/quick/EvalPointMovesCmdTest

CONFIG="-C Testing.conf"

outputdir=$HOOT_HOME/test-output/cmd/quick/EvalPointMovesCmdTest
myCmd="hoot evaluate-point-moves $CONFIG --fatal 10 1.0,1.0,5.0,5.0 $outputdir"

# Run test
$myCmd

