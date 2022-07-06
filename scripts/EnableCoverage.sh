#!/bin/bash

set -e

# This script can be run at different times so it should update both
# the original LocalConfig.pri.orig and the copy LocalConfig.pri
function update_local_config {
  config_path=$1
  if [ -w $config_path ]
  then
    # Coverage only works on a debug build
    # Turn off release mode
    sed -i 's/\(CONFIG += release\)/#\1/g' $config_path
    sed -i 's/\(CONFIG -= debug\)/#\1/g'   $config_path
    # Turn on debug mode
    sed -i 's/#\(CONFIG -= release\)/\1/g' $config_path
    sed -i 's/#\(CONFIG += debug\)/\1/g'   $config_path
  fi
}

# Enable coverage in VagrantBuild script
sed -i 's/\(--with-uitests\)/\1 --with-coverage/g' $HOOT_HOME/VagrantBuild.sh

# Update the config files to build in debug mode
update_local_config $HOOT_HOME/LocalConfig.pri
update_local_config $HOOT_HOME/LocalConfig.pri.orig

exit 0
