#!/bin/bash
set -e

mkdir -p test-output/cmd/csr

# PRE TEST FILE GENERATION 

hoot conflate -D writer.include.debug=true -D stats.class="hoot::ScriptStatsComposer" -D  stats.script=./report/csr-default.js -D stats.format=asciidoc -D stats.output=test-output/cmd/csr/csr ./test-files/conflate/unified/AllDataTypesA.osm ./test-files/conflate/unified/AllDataTypesB.osm test-output/cmd/csr/csr.osm --stats >& /dev/null

# TEST 1: integrity check of the output osm file when enabling Report Generation and stats
#  - will produce difference which will make different from baseline
hoot conflate -D writer.include.debug=true -D stats.class="hoot::ScriptStatsComposer" -D  stats.script=./report/csr-default.js -D stats.format=html -D stats.output=test-output/cmd/csr/csr ./test-files/ToyTestA.osm ./test-files/ToyTestB.osm test-output/cmd/csr/csr2.osm --stats >& /dev/null
hoot --is-match test-output/cmd/csr/csr.osm test-files/cmd/slow/csr/csr.osm || diff test-output/cmd/csr/csr.osm test-files/cmd/slow/csr/csr.osm 
echo "completed test 1"

# TEST 2: simple file test to make sure asciidoc created
FILE_ASCIIDOC=test-output/cmd/csr/csr.asciidoc
if [ ! -f $FILE_ASCIIDOC ]; then
  echo "ConflationStatsReportTest: asciidoc file not generated"
else
  echo "passed test 2"
fi

# TEST 3: simple file test to make sure html created
FILE_HTML=test-output/cmd/csr/csr.html
if [ ! -f $FILE_HTML ]; then
  echo "ConflationStatsReportTest: html file not generated"
else
  echo "passed test 3"
fi

# TEST 4: simple file test to make sure pdf created
hoot conflate -D writer.include.debug=true -D stats.class="hoot::ScriptStatsComposer" -D  stats.script=./report/csr-default.js -D stats.format=pdf -D stats.output=test-output/cmd/csr/csr ./test-files/ToyTestA.osm ./test-files/ToyTestB.osm test-output/cmd/csr/csr2.osm --stats >& test-output/cmd/csr/csrStats.txt
FILE_PDF=test-output/cmd/csr/csr.pdf
if [ ! -f $FILE_PDF ]; then
  echo "ConflationStatsReportTest: pdf file not generated"
else
  echo "passed test 4"
fi

# Make sure the expected files are listed.
echo Output files:
# For LC_ALL explation see:
# https://github.com/ngageoint/hootenanny-rpms/issues/41
ls test-output/cmd/csr/ | LC_ALL=C sort

# Size on yubyub, Ubuntu 14.04, 2016-02-01 - 192002
PDF_SIZE=`wc -c test-output/cmd/csr/csr.pdf | cut -d' ' -f1`
[ $PDF_SIZE -lt 150000 ] && echo PDF is smaller than expected
[ $PDF_SIZE -gt 350000 ] && echo PDF is bigger than expected
[ $PDF_SIZE -gt 150000 ] && [ $PDF_SIZE -lt 350000 ] && echo PDF size in range.

# TEST 5: test a sampling of the stats in the output of the stats command
# - will output difference if fails
python scripts/core/CheckThreeScores.py test-output/cmd/csr/csrStats.txt "Node Count,35,37,64,66,36,38" "Way Count,3,5,3,5,8,10" "Highway Count,3,5,3,5,8,10" "Total Unmatched Features,3,5,3,5,0,1"
echo "completed test 5"

