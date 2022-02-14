#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/ConflateCmdStatsTest
OUT_DIR=test-output/cmd/slow/ConflateCmdStatsTest
mkdir -p $OUT_DIR
STATS_OUT=$OUT_DIR/toy-roads-out
SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

CONFIG="-C Testing.conf -C UnifyingAlgorithm.conf -C ReferenceConflation.conf -D writer.include.debug.tags=true"
source scripts/core/ScriptTestUtils.sh

hoot conflate $CONFIG -D match.creators="HighwayMatchCreator" -D merger.creators="HighwayMergerCreator" \
  test-files/ToyTestA.osm test-files/ToyTestB.osm $STATS_OUT.osm --stats > $STATS_OUT
hoot diff $CONFIG $STATS_OUT.osm $IN_DIR/toy-roads-out.osm || diff $STATS_OUT.osm $IN_DIR/toy-roads-out.osm

# Read in a set of stat names from a file, delete them from the hoot command stats output, and write 
# the remaining stats to the final output.
EDIT_CMD=""
while read line
do
  #replace new lines with spaces and then remove the trailing whitespace
  MODIFIED_LINE=`echo "$line"|tr '\n' ' '|sed -e 's/[ \t]*$//'`
  EDIT_CMD=$EDIT_CMD" -e '/"$MODIFIED_LINE"/d'"
done < $SCRIPT_DIR/ConflateCmdStatsTest.conf
EDIT_CMD="sed "$EDIT_CMD" "$STATS_OUT" | sed -n -e '/stats =/,\$p'"
eval $EDIT_CMD
