#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/quick/EvalMoveCmdTest

outputdir=$HOOT_HOME/test-output/cmd/quick/EvalMoveCmdTest
myCmd="hoot evaluate-point-moves --fatal 10 1.0,1.0,5.0,5.0 $outputdir"

# Run test
$myCmd

