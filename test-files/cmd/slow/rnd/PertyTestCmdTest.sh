#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/PertyTestCmdTest

infile=$HOOT_HOME/test-files/DcGisRoads.osm
outdir=$HOOT_HOME/test-output/cmd/slow/PertyTestCmdTest

myCmd="hoot perty-test \
        -D perty.test.num.runs=1 \
        -D perty.test.num.simulations=1 \
        -D perty.test.expected.scores=0.5 \
        -D perty.test.allowed.score.variance=0.55 \
        $infile $outdir"

pass=`$myCmd | grep "Test Passed:" | sed "s/Test Passed: //g;"`

if [ $pass = "true" ]; then
  echo "PERTY Test Passed"
else
  cleanCmd=`echo $myCmd | sed "s/\s+?/ /g"`  
  printf "PERTY Test Failed! Try running\n\n$cleanCmd\n\nto find out why\n\n"
fi 

