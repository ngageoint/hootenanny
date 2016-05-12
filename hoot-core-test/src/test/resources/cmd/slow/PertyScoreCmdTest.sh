#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/PertyScorCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
outdir=$HOOT_HOME/test-output/cmd/slow/PertyCmdTest/

myCmd="hoot perty-score -D perty.systematic.error.x=1 \
                        -D perty.systematic.error.y=1 \
                        $inputfile $outdir"
score=`$myCmd | grep "PERTY" | sed "s/PERTY Score: //g"`
target_score="0.950"

result=`echo $score'>='$target_score | bc`

if [ result ]; then
  echo "PERTY score greather than or equal to $target_score"
else
  echo "PERTY score ($score) LESS THAN $target_score"
fi

