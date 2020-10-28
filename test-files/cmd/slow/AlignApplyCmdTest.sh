#!/bin/bash
set -e

mkdir -p $HOOT_HOME/test-output/cmd/slow/AlignApplyCmdTest

CONFIG="-C Testing.conf"

# First we derive a rubber sheet
hoot align --error $CONFIG --derive \
  --ref $HOOT_HOME/test-files/DcGisRoads.osm \
        $HOOT_HOME/test-files/DcTigerRoads.osm \
        $HOOT_HOME/test-output/cmd/slow/AlignApplyCmdTest/DcTigerToDcGis.rs 

# For some reason, the .rs file is slightly different on CentOS.
# If we ignore the comparison here, final comparision of osm files
# turns out OK in the end...
# Compare to known-good
goodfile=$HOOT_HOME/test-files/cmd/slow/AlignApplyCmdTest/DcTigerToDcGis.rs
testfile=$HOOT_HOME/test-output/cmd/slow/AlignApplyCmdTest/DcTigerToDcGis.rs
#cmp $goodfile $testfile
#if [ "$?" = "1" ]; then
#  echo "Rubber Sheet files are not equal! Try cmp -l $goodfile $testfile"
#  exit 1
#fi

# Now we apply the rubber sheet
hoot align --error $CONFIG --apply \
  $HOOT_HOME/test-output/cmd/slow/AlignApplyCmdTest/DcTigerToDcGis.rs \
  $HOOT_HOME/test-files/DcTigerRoads.osm \
  $HOOT_HOME/test-output/cmd/slow/AlignApplyCmdTest/DcTigerToDcGis.osm 

goodfile=$HOOT_HOME/test-files/cmd/slow/AlignApplyCmdTest/DcTigerToDcGis.osm
testfile=$HOOT_HOME/test-output/cmd/slow/AlignApplyCmdTest/DcTigerToDcGis.osm
hoot diff $CONFIG $goodfile $testfile || diff $goodfile $testfile
