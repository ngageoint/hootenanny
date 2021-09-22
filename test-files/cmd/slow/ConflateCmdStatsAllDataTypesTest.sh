#!/bin/bash
set -e

# This test collects statistics on a variety of data types.
#
# Note: The input data doesn't have *all* of the data types hoot is able to conflate. Purposefully
# not reducing any conflate matchers here so we can see how stats for types with overlapping 
# properties get handled.

IN_DIR=test-files/cmd/slow/ConflateCmdStatsTest
IN_DIR_2=test-files/conflate/unified
OUT_DIR=test-output/cmd/slow/ConflateCmdStatsTest
mkdir -p $OUT_DIR
STATS_OUT=$OUT_DIR/all-data-types-out
SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

CONFIG="-C Testing.conf"

TO_VALIDATE_1=test-output/cmd/slow/ConflateCmdStatsTest/all-data-types-out.osm
VALIDATION_REPORT_GOLD_1=test-files/cmd/slow/ConflateCmdStatsTest/all-data-types-out-validation-report
hoot conflate $CONFIG -C UnifyingAlgorithm.conf -C ReferenceConflation.conf \
  -D writer.include.debug.tags=true $IN_DIR_2/AllDataTypesA.osm $IN_DIR_2/AllDataTypesB.osm \
  $STATS_OUT.osm --stats > $STATS_OUT
hoot diff $CONFIG $STATS_OUT.osm $IN_DIR/all-data-types-out.osm || diff $STATS_OUT.osm $IN_DIR/all-data-types-out.osm

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
