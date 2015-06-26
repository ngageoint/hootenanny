#!/bin/bash 
#
# Script to generate the Conflation Statistics Report with a title page, TOC, and the main sections.
# Usage: makeReference.sh <input1_path> <input2_path> <output_path> "extra args wrap in 1 pair quotes"
#
# sample run: ./makeReference.sh $HOOT_HOME/test-files/ToyTestA.osm $HOOT_HOME/test-files/ToyTestB.osm /tmp/output1.osm

usageCmd="usage: makeReference.sh <input1> <input2> <output> <report_output> [optional 'args']"
usageSample="       makeReference.sh TestSample | TestBuildings | TestRoads | TestPois | TestMixed | TestTypes | AllTests"

if [ $# -lt 4 ]
then 
  if [ $# -eq 1 ]
  then
    if [ "$1" = "TestSample" ]
    then ./makeReference.sh $HOOT_HOME/test-files/ToyTestA.osm $HOOT_HOME/test-files/ToyTestB.osm $HOOT_HOME/report/TestSampleRef.osm $HOOT_HOME/report/TestSampleRef.pdf
    elif [ "$1" = "TestBuildings" ]
    then ./makeReference.sh $HOOT_HOME/test-files/ToyBuildingsTestA.osm $HOOT_HOME/test-files/ToyBuildingsTestB.osm $HOOT_HOME/report/TestBuildingsRef.osm $HOOT_HOME/report/TestBuildingsRef.pdf
    elif [ "$1" = "TestRoads" ]
    then ./makeReference.sh $HOOT_HOME/test-files/DcGisRoads.osm $HOOT_HOME/test-files/DcTigerRoads.osm $HOOT_HOME/report/TestRoadsRef.osm $HOOT_HOME/report/TestRoadsRef.pdf
    elif [ "$1" = "TestPois" ]
    then ./makeReference.sh $HOOT_HOME/test-files/conflate/point/Poi1.osm $HOOT_HOME/test-files/conflate/point/Poi2.osm $HOOT_HOME/report/TestPoisRef.osm $HOOT_HOME/report/TestPoisRef.pdf
    elif [ "$1" = "TestMixed" ]
    then ./makeReference.sh $HOOT_HOME/test-files/conflate/unified/AllDataTypesA.osm $HOOT_HOME/test-files/conflate/unified/AllDataTypesB.osm $HOOT_HOME/report/TestMixedRef.osm $HOOT_HOME/report/TestMixedRef.pdf
    elif [ "$1" = "TestTypes" ]
    then ./makeReference.sh $HOOT_HOME/test-files/conflate/unified/AllDataTypesA.osm $HOOT_HOME/test-files/conflate/unified/AllDataTypesB.osm $HOOT_HOME/report/TestTypesRef.osm $HOOT_HOME/report/TestTypesRef.pdf '-D conflate.stats.types="reference"'
    elif [ "$1" = "AllTests" ]
    then 
      ./makeReference.sh TestSample
      ./makeReference.sh TestBuildings
      ./makeReference.sh TestRoads
      ./makeReference.sh TestPois
      ./makeReference.sh TestMixed
      ./makeReference.sh TestTypes
    else 
      echo $usageCmd
      echo $usageSample
    fi
  else 
    echo $usageCmd
    echo $usageSample
  fi
  exit 1
fi

cd ..
args=
if [ $# -gt 4 ]
  then args=$5
fi

cmd1="hoot conflate -D writer.include.debug=true -D stats.class=\"hoot::ScriptStatsComposer\" -D stats.script=$HOOT_HOME/report/csr-reference.js"
cmd2="-D stats.format=asciidoc -D stats.output=./report/referenceBody $args $1"
cmd3="$2 $3 --stats"
echo $cmd1 $cmd2 $cmd3
hoot conflate -D writer.include.debug=true -D stats.class="hoot::ScriptStatsComposer" -D stats.script=$HOOT_HOME/report/csr-reference.js -D stats.format=asciidoc -D stats.output=./report/referenceBody $args $1 $2 $3 --stats

cd report

# strip the path off the args $1, $2, $3
basenameInput1=`awk 'END{ var=FILENAME; n=split (var,a,/\//); print a[n]}' $1`
basenameInput2=`awk 'END{ var=FILENAME; n=split (var,a,/\//); print a[n]}' $2`
basenameOutput=`awk 'END{ var=FILENAME; n=split (var,a,/\//); print a[n]}' $3`

echo a2x -a docinfo --dblatex-opts "-P latex.output.revhistory=0 -P latex.unicode.use=1 -s reportStyle.sty --param doc.publisher.show=0" -a HasLatexMath -a 'revdate=v`$HOOT_HOME/bin/hoot version --error | sed "s/Hootenanny \([^ ]*\) Built.*/\\1/g"`, `date "+%B %d, %Y"`' -a "input1=$basenameInput1" -a "input2=$basenameInput2" -a "output=$basenameOutput" -a "args=$args" -v -f pdf reference.asciidoc
a2x -a docinfo --dblatex-opts "-P latex.output.revhistory=0 -P latex.unicode.use=1 -s reportStyle.sty --param doc.publisher.show=0" -a HasLatexMath -a 'revdate=v`$HOOT_HOME/bin/hoot version --error | sed "s/Hootenanny \([^ ]*\) Built.*/\\1/g"`, `date "+%B %d, %Y"`' -a "input1=$basenameInput1" -a "input2=$basenameInput2" -a "output=$basenameOutput" -a "args=$args" -a "cmd1=$cmd1" -a "cmd2=$cmd2" -a "cmd3=$cmd3" -v -f pdf reference.asciidoc

if [ $# -gt 3 ]
  then cp reference.pdf $4
fi
