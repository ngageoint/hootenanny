#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/slow/RndOptimizeNetworkConflateSettingsCmdTest
OUTPUT_FILE=RndOptimizeNetworkConflateSettingsCmdTest-states-out
mkdir -p $OUTPUT_DIR
rm -f $OUTPUT_DIR/$OUTPUT_FILE

# we're not verifying output here...just making sure the command runs to completion and the output exists
hoot optimize-network-conf --error "test-files/cmd/slow/RndOptimizeNetworkConflateSettingsCmdTest/static-tests" "case" "test-files/cmd/slow/RndOptimizeNetworkConflateSettingsCmdTest/testSettings.json" 2 $OUTPUT_DIR/$OUTPUT_FILE "false" "test-files/cases/hoot-rnd/network/Config.conf"

if [ ! -f $OUTPUT_DIR/$OUTPUT_FILE ]; then
  echo $OUTPUT_DIR/$OUTPUT_FILE " not found." 
  exit -1
fi
