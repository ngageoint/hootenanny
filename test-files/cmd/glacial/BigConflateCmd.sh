#!/bin/bash

set -e

function pass {
    touch /dev/null
}

TEST_FILES=test-files/

echo "Remove anything left over from a previous test run. We don't care if these fail."
hadoop fs -rmr DcTigerRoads.osm.pbf &> /dev/null || pass
hadoop fs -rmr DcGisRoads.osm.pbf &> /dev/null || pass
hadoop fs -rmr DcConflated.osm.pbf &> /dev/null || pass
hadoop fs -rmr DcConflated.osm.pbf-cache &> /dev/null || pass
rm -f test-output/DcBigConflated.osm.pbf

echo "Staging our data on HDFS"
hadoop fs -mkdir DcTigerRoads.osm.pbf DcGisRoads.osm.pbf
hadoop fs -put $TEST_FILES/DcTigerRoads.pbf DcTigerRoads.osm.pbf/
hadoop fs -put $TEST_FILES/DcGisRoads.pbf DcGisRoads.osm.pbf/

echo "Run our conflation"
hoot --big-conflate DcTigerRoads.osm.pbf DcGisRoads.osm.pbf DcConflated.osm.pbf

echo "Read the data out of HDFS."
hadoop fs -cat DcConflated.osm.pbf/*.pbf | hoot --add-pbf-header -  test-output/DcBigConflated.osm.pbf

echo "Print out some statistics. This should reveal any gross errors."
mkdir -p test-output/cmd/glacial/
rm -f test-output/cmd/glacial/BigConflateStats.log
hoot --stats --warn -D stats.translate.script=$HOOT_HOME/translations/HootTest.js test-output/DcBigConflated.osm.pbf > test-output/cmd/glacial/BigConflateStats.log
python scripts/core/CheckScores.py test-output/cmd/glacial/BigConflateStats.log "Node Count,7578,7580" "Way Count,755,756" "Meters of Highway,70095,70180"

