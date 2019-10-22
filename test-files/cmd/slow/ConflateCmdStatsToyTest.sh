#!/bin/bash
set -e

mkdir -p test-output/cmd/slow/ConflateCmdStatsTest
STATS_OUT=test-output/cmd/slow/ConflateCmdStatsTest/toy-out
SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

HOOT_OPTS="-C Testing.conf -C UnifyingAlgorithm.conf -C ReferenceConflation.conf -D writer.include.debug.tags=true"

hoot conflate $HOOT_OPTS test-files/ToyTestA.osm test-files/ToyTestB.osm $STATS_OUT.osm --stats > $STATS_OUT

#read in a set of stat names from a file, delete them from the hoot command stats output, and write the remaining stats to the final output
EDIT_CMD=""
while read line
do
  #replace new lines with spaces and then remove the trailing whitespace
  MODIFIED_LINE=`echo "$line"|tr '\n' ' '|sed -e 's/[ \t]*$//'`
  EDIT_CMD=$EDIT_CMD" -e '/"$MODIFIED_LINE"/d'"
done < $SCRIPT_DIR/ConflateCmdStatsTest.conf
EDIT_CMD="sed "$EDIT_CMD" "$STATS_OUT" | sed -n -e '/stats =/,\$p'"
eval $EDIT_CMD
