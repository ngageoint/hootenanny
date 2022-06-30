#!/bin/bash

set -e

# Enable coverage in VagrantBuild script
sed -i 's/--with-uitests/--with-uitests --with-coverage/g' $HOOT_HOME/VagrantBuild.sh
# Update compiler optimization flags for use with building coverage
sed -i 's/QMAKE_CXXFLAGS+=-O3/QMAKE_CXXFLAGS+=-O1/' $HOOT_HOME/LocalConfig.pri.orig
if [ -w $HOOT_HOME/LocalConfig.pri ]
then
  sed -i 's/QMAKE_CXXFLAGS+=-O3/QMAKE_CXXFLAGS+=-O1/' $HOOT_HOME/LocalConfig.pri
fi

exit 0
