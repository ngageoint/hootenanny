#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/EvalMoveCmdTest

outputdir=$HOOT_HOME/test-output/cmd/slow/EvalMoveCmdTest
myCmd="hoot eval-move --fatal 10 1.0,1.0,5.0,5.0 $outputdir"

# Run test
$myCmd

