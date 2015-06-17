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

# TEST 5: test a sampling of the stats in the output of the stats command
# - will output difference if fails
python scripts/CheckThreeScores.py test-output/cmd/csr/csrStats.txt "Node Count,35,37,64,66,36,38" "Way Count,3,5,3,5,8,10" "Highway Count,3,5,3,5,8,10" "Total Unmatched Features,3,5,3,5,0,0"
echo "completed test 5"

# TEST 6: test all of the tokens used in the report.  make sure they are all there so someone cannot break the code
#   by renaming the stats tokens at random
# - will output difference if fails
python scripts/CheckTokens.py test-output/cmd/csr/csrStats.txt 'Node Count' 'Way Count' 'Relation Count' 'Total Feature Tags' 'Total Feature Information Tags' 'Total Feature Metadata Tags' 'Unique Names' 'Meters of Linear Features' 'Meters Squared of Area Features' 'POI Count' 'Highway Count' 'Meters of Highway' 'Highway Unique Name Count' 'Total Building Count' 'Meters Squared of Buildings' 'Building Unique Name Count' 'Total Feature Count' 'Total Conflated Features' 'Percentage of Total Features Conflated' 'Total Unmatched Features' 'Percentage of Total Features Unmatched' 'Conflated POIs' 'Conflated Highways' 'Meters of Conflated Highway' 'Conflated Buildings' 'Meters Squared of Conflated Buildings' 'Total Features Marked for Review' 'Percentage of Total Features Marked for Review' 'Longest Tag' 'Translated Populated Tag Percent' 'Translated Populated Tag Count' 'Translated Default Tag Count' 'Translated Null Tag Count' 'Building Translated Populated Tag Percent' 'Highway Translated Populated Tag Percent' 'POI Translated Populated Tag Percent' 'Read Inputs Time (sec)' 'Time to Calculate Stats for Input 1 (sec)' 'Time to Calculate Stats for Input 2 (sec)' 'Apply Named Ops Time (sec)' 'Apply Pre Ops Time (sec)' 'Project to Planar Time (sec)' 'Find Matches Time (sec)' 'Number of Matches Found' 'Number of Matches Found per Second' 'Number of Matches Before Whole Groups' 'Number of Whole Groups' 'Optimize Matches Time (sec)' 'Number of Optimized Matches' 'Number of Matches Optimized per Second' 'Create Mergers Time (sec)' 'Apply Mergers Time (sec)' 'Mergers Applied per Second' 'Apply Post Ops Time (sec)' 'Conflation Time (sec)' 'Project to WGS84 Time (sec)' 'Time to Calculate Stats for Output (sec)' 'Write Output Time (sec)' 'Total Time Elapsed (sec)' 'Bytes Read' 'Bytes Written'
