#!/usr/bin/env bash

# Wrapper script to start tomcat8
export HOOT_HOME=/home/vagrant/hoot
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:$HOOT_HOME/lib:$HOOT_HOME/pretty-pipes/lib

# Take into account different locations when GDAL installed from source
# versus package.
export GDAL_DATA=$(gdal-config --datadir)
if [ "$(gdal-config --prefix)" == "/usr/local" ]; then
    export GDAL_LIB_DIR=/usr/local/lib
else
    export GDAL_LIB_DIR=$(pkg-config gdal --variable libdir)
fi
export HOOT_WORKING_NAME=hoot
export PATH=$HOOT_HOME/bin:$PATH

# The actual run script
$CATALINA_HOME/bin/catalina.sh start
