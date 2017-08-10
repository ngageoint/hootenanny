#!/bin/bash
set -e

OUTPUT_DIR=test-output/cmd/glacial/OptimizeConflateSettingsCmdTest
OUTPUT_FILE=OptimizeConflateSettingsCmdTest-states-out
mkdir -p $OUTPUT_DIR
rm -f $OUTPUT_DIR/$OUTPUT_FILE

# This test is only meant to be run --with-rnd and hoot-rnd doesn't run its own test suite.  
# So, I'm checking to see if the command is in the list of hoot commands and exiting if it 
# doesn't...there may be a better way to handle this.  This will throw off the test count, unfortunately.
OPT_COMMAND=`hoot | grep optimize-conflate-settings`
#echo $OPT_COMMAND
if [ -z "$OPT_COMMAND" ]; then
  #echo "optimize-conflate-settings command not present.  Exiting test without running it."
 exit 0
fi

# we're not verifying output here...just making sure the command runs to completion and the output exists
hoot optimize-conflate-settings --error "test-files/cases/hoot-rnd/network/conflicts" "case" "test-files/cmd/glacial/OptimizeConflateSettingsCmd/testSettings.json" 2 $OUTPUT_DIR/$OUTPUT_FILE "false" "test-files/cases/hoot-rnd/network/Config.conf"

if [ ! -f $OUTPUT_DIR/$OUTPUT_FILE ]; then
  echo $OUTPUT_DIR/$OUTPUT_FILE " not found." 
  exit -1
fi
