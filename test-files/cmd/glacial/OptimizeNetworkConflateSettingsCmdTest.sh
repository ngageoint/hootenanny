#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/glacial/OptimizeNetworkConflateSettingsCmdTest
OUTPUT_FILE=OptimizeNetworkConflateSettingsCmdTest-states-out
mkdir -p $OUTPUT_DIR
rm -f $OUTPUT_DIR/$OUTPUT_FILE

# we're not verifying output here...just making sure the command runs to completion and the output exists
# TODO: Should the config be changed to test-files/cases/network/Config.conf?
hoot optimize-network-conf --error -C Testing.conf "test-files/cmd/glacial/OptimizeNetworkConflateSettingsCmdTest/static-tests" "case" "test-files/cmd/glacial/OptimizeNetworkConflateSettingsCmdTest/testSettings.json" 2 $OUTPUT_DIR/$OUTPUT_FILE "false" "test-files/cases/hoot-rnd/network/Config.conf"

if [ ! -f $OUTPUT_DIR/$OUTPUT_FILE ]; then
  echo $OUTPUT_DIR/$OUTPUT_FILE " not found." 
  exit -1
fi
