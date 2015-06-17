#!/bin/bash 
#
# Script to generate the Conflation Statistics Report with a title page, TOC, and the main sections.
# Usage: makeReport.sh <input1_path> <input2_path> <output_path> "extra args wrap in 1 pair quotes"
#
# sample run: ./makeReport.sh $HOOT_HOME/test-files/ToyTestA.osm $HOOT_HOME/test-files/ToyTestB.osm /tmp/output1.osm

usageCmd="usage: makeReport.sh <input1> <input2> <output> <report_output> [optional 'args']"
usageSample="       makeReport.sh TestSample | TestBuildings | TestRoads | TestPois | TestMixed | TestTypes | AllTests"

if [ $# -lt 4 ]
then 
  if [ $# -eq 1 ]
  then
    if [ "$1" = "TestSample" ]
    then ./makeReport.sh $HOOT_HOME/test-files/ToyTestA.osm $HOOT_HOME/test-files/ToyTestB.osm $HOOT_HOME/report/TestSample.osm $HOOT_HOME/report/TestSample.pdf
    elif [ "$1" = "TestBuildings" ]
    then ./makeReport.sh $HOOT_HOME/test-files/ToyBuildingsTestA.osm $HOOT_HOME/test-files/ToyBuildingsTestB.osm $HOOT_HOME/report/TestBuildings.osm $HOOT_HOME/report/TestBuildings.pdf
    elif [ "$1" = "TestRoads" ]
    then ./makeReport.sh $HOOT_HOME/test-files/DcGisRoads.osm $HOOT_HOME/test-files/DcTigerRoads.osm $HOOT_HOME/report/TestRoads.osm $HOOT_HOME/report/TestRoads.pdf
    elif [ "$1" = "TestPois" ]
    then ./makeReport.sh $HOOT_HOME/test-files/conflate/point/Poi1.osm $HOOT_HOME/test-files/conflate/point/Poi2.osm $HOOT_HOME/report/TestPois.osm $HOOT_HOME/report/TestPois.pdf
    elif [ "$1" = "TestMixed" ]
    then ./makeReport.sh $HOOT_HOME/test-files/conflate/unified/AllDataTypesA.osm $HOOT_HOME/test-files/conflate/unified/AllDataTypesB.osm $HOOT_HOME/report/TestMixed.osm $HOOT_HOME/report/TestMixed.pdf
    elif [ "$1" = "TestTypes" ]
    then ./makeReport.sh $HOOT_HOME/test-files/conflate/unified/AllDataTypesA.osm $HOOT_HOME/test-files/conflate/unified/AllDataTypesB.osm $HOOT_HOME/report/TestTypes.osm $HOOT_HOME/report/TestTypes.pdf '-D conflate.stats.types="reference"'
    elif [ "$1" = "AllTests" ]
    then 
      ./makeReport.sh TestSample
      ./makeReport.sh TestBuildings
      ./makeReport.sh TestRoads
      ./makeReport.sh TestPois
      ./makeReport.sh TestMixed
      ./makeReport.sh TestTypes
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

cmd1="hoot conflate -D writer.include.debug=true -D stats.class=\"hoot::ScriptStatsComposer\""
cmd2="-D stats.format=asciidoc -D stats.output=./report/reportBody $args $1"
cmd3="$2 $3 --stats"
echo $cmd1 $cmd2 $cmd3
hoot conflate -D writer.include.debug=true -D stats.class="hoot::ScriptStatsComposer" -D stats.format=asciidoc -D stats.output=./report/reportBody $args $1 $2 $3 --stats

cd report

# strip the path off the args $1, $2, $3
basenameInput1=`awk 'END{ var=FILENAME; n=split (var,a,/\//); print a[n]}' $1`
basenameInput2=`awk 'END{ var=FILENAME; n=split (var,a,/\//); print a[n]}' $2`
basenameOutput=`awk 'END{ var=FILENAME; n=split (var,a,/\//); print a[n]}' $3`

echo a2x -a docinfo --dblatex-opts "-P latex.output.revhistory=0 -P latex.unicode.use=1 -s reportStyle.sty --param doc.publisher.show=0" -a HasLatexMath -a 'revdate=v`$HOOT_HOME/bin/hoot version --error | sed "s/Hootenanny \([^ ]*\) Built.*/\\1/g"`, `date "+%B %d, %Y"`' -a "input1=$basenameInput1" -a "input2=$basenameInput2" -a "output=$basenameOutput" -a "args=$args" -v -f pdf report.asciidoc
a2x -a docinfo --dblatex-opts "-P latex.output.revhistory=0 -P latex.unicode.use=1 -s reportStyle.sty --param doc.publisher.show=0" -a HasLatexMath -a 'revdate=v`$HOOT_HOME/bin/hoot version --error | sed "s/Hootenanny \([^ ]*\) Built.*/\\1/g"`, `date "+%B %d, %Y"`' -a "input1=$basenameInput1" -a "input2=$basenameInput2" -a "output=$basenameOutput" -a "args=$args" -a "cmd1=$cmd1" -a "cmd2=$cmd2" -a "cmd3=$cmd3" -v -f pdf report.asciidoc

if [ $# -gt 3 ]
  then cp report.pdf $4
fi
