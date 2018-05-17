#!/bin/bash
set -e

# rnd command
OPT_COMMAND=`hoot | grep eval-point-moves`
if [ -z "$OPT_COMMAND" ]; then
 exit 0
fi

mkdir -p $HOOT_HOME/test-output/cmd/quick/EvalPointMovesCmdTest

outputdir=$HOOT_HOME/test-output/cmd/quick/EvalPointMovesCmdTest
myCmd="hoot evaluate-point-moves --fatal 10 1.0,1.0,5.0,5.0 $outputdir"

# Run test
$myCmd

