#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/glacial/RndPertyTestCmdTest

infile=$HOOT_HOME/test-files/DcGisRoads.osm
outdir=$HOOT_HOME/test-output/cmd/glacial/RndPertyTestCmdTest

myCmd="hoot perty -C Testing.conf \
        -D perty.test.num.runs=1 \
        -D perty.test.num.simulations=1 \
        -D perty.test.expected.scores=0.5 \
        -D perty.test.allowed.score.variance=0.55 \
        $infile $outdir --test"

pass=`$myCmd | grep "Test Passed:" | sed "s/Test Passed: //g;"`

if [ $pass = "true" ]; then
  echo "PERTY Test Passed"
else
  cleanCmd=`echo $myCmd | sed "s/\s+?/ /g"`  
  printf "PERTY Test Failed! Try running\n\n$cleanCmd\n\nto find out why\n\n"
fi 

