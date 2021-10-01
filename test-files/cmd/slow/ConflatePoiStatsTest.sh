#!/bin/bash
set -e

IN_DIR=test-files/cmd/slow/ConflatePoiStatsTest
OUT_DIR=test-output/cmd/slow/ConflatePoiStatsTest
mkdir -p $OUT_DIR
STATS_OUT=$OUT_DIR/poi-out
SCRIPT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
source scripts/core/ScriptTestUtils.sh

hoot conflate -C Testing.conf -D writer.include.debug.tags=true \
  -D "match.creators=ScriptMatchCreator,Poi.js" -D "merger.creators=ScriptMergerCreator" \
  test-files/dcpoi_clip.osm test-files/mapcruzinpoi_clip.osm $STATS_OUT.osm --stats > $STATS_OUT
  
validateTestOutput $OUT_DIR/poi-out.osm $OUT_DIR/poi-out-validation-report \
  $OUT_DIR/poi-out-validated.osm $IN_DIR/poi-out-validation-report

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
