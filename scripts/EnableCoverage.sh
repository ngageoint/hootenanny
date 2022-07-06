#!/bin/bash

set -e

function update_local_config {
  config_path=$1
  if [ -w $config_path ]
  then
    # Coverage only works on a debug build
    # Turn off release mode
    sed -i 's/\(CONFIG += release\)/#1/g' $config_path
    sed -i 's/\(CONFIG -= debug\)/#1/g'   $config_path
    # Turn on debug mode
    sed -i 's/#\(CONFIG -= release\)/1/g' $config_path
    sed -i 's/#\(CONFIG += debug\)/1/g'   $config_path
  fi
}

# Enable coverage in VagrantBuild script
sed -i 's/--with-uitests/--with-uitests --with-coverage/g' $HOOT_HOME/VagrantBuild.sh
# Update the config files to build in debug mode
update_local_config $HOOT_HOME/LocalConfig.pri
update_local_config $HOOT_HOME/LocalConfig.pri.orig

exit 0
