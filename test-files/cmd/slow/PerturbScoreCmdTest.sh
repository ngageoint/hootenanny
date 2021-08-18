#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/PerturbScoreCmdTest

inputfile=$HOOT_HOME/test-files/DcGisRoads.osm
outdir=$HOOT_HOME/test-output/cmd/slow/PerturbScoreCmdTest/

myCmd="hoot perturb -C Testing.conf -D random.seed=1 -D perty.systematic.error.x=1 \
                        -D perty.systematic.error.y=1 \
                        $inputfile $outdir --score"
score=`$myCmd | grep "PERTY" | sed "s/PERTY Score: //g"`
target_score="0.950"

# Commented out since we don't have "bc" everywhere
# result=`echo $score'>='$target_score | bc`
# if [ result ]; then
if [ $(awk '{printf($1 >= $2) ? 1 : 0}' <<<" $score $target_score ") -eq "1" ] ; then
  echo "PERTY score greater than or equal to $target_score"
else
  echo "PERTY score ($score) LESS THAN $target_score"
fi
