#!/bin/bash

set -e

function pass {
    touch /dev/null
}

TEST_FILES=test-files/cmd/glacial/

echo "Remove anything left over from a previous test run. We don't care if these fail."
hadoop fs -rmr BigPertyTest* &> /dev/null || pass
rm -f test-output/BigPertyTestOutput.osm.pbf

echo "Staging our data on HDFS"
hadoop fs -mkdir BigPertyTestInput.osm.pbf
hadoop fs -put $TEST_FILES/BigPertyCmd.osm.pbf BigPertyTestInput.osm.pbf/

echo "Run big perty"
# Force tiles artificially
hoot --big-perty -D hoot.hadoop.pixelSize=0.001 -D hoot.hadoop.maxNodeCount=2000 1 BigPertyTestInput.osm.pbf BigPertyTestOutput.osm.pbf --local
#hoot --big-perty 1 BigPertyTestInput.osm.pbf BigPertyTestOutput.osm.pbf --local

echo "Read the data out of HDFS."
hadoop fs -cat BigPertyTestOutput.osm.pbf/*.pbf | hoot --add-pbf-header -  test-output/BigPertyTestOutput.osm.pbf

echo "Check to see if the files match."
hoot --is-match $TEST_FILES/BigPertyCmdOutput.osm.pbf test-output/BigPertyTestOutput.osm.pbf

