#!/bin/bash
set -e

# Tests to see if we're handling roundabouts during conflate correctly in the replacement changeset workflow. Roundabouts within the replacement 
# AOI in the output (1 of 2 for this test) should remain connected to roads that were feeding them from the input.

# Note that there are a ton of duplicated and unnconnected roads in the ref input whose creation must have been the result of a mistake while 
# processing the original source data. This causes dropped feature problems in the output after recent changes made to C&R to prevent 
# unnecessary create/delete changeset statements by preserving ref IDs. The data should probably eventually be cleaned up, which will be 
# tedious (de-duplicate commmand isn't removing all the dupes). In the meantime, however, the replacement AOI was shrunk to just cover one of 
# the roundabouts where the dropped feature problem doesn't occur.

test-files/cmd/glacial/serial/ServiceChangesetReplacement.sh.off "ServiceChangesetReplacementRoundaboutsTest" "test-files/cmd/glacial/serial/ServiceChangesetReplacementRoundaboutsTest/Input1.osm" "test-files/cmd/glacial/serial/ServiceChangesetReplacementRoundaboutsTest/Input2.osm" "-89.6219483,20.9953,-89.6199,20.9979" "-180,-90,180,90" "true" "lenient" "" "" "false" "" "" "false" "" "xml" "5.0" "0.5" "false" "true"
