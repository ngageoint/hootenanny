#!/bin/bash

SCRIPT_FILE=`readlink -f ${BASH_SOURCE[0]}`
SCRIPT_PATH="$( cd "$( dirname "$SCRIPT_FILE" )" && pwd )"

export HOOT_HOME=`readlink -m $SCRIPT_PATH/../`
export HOOT_WORKING_NAME=`echo $HOOT_HOME | sed -e "s/.*\///g"`

# Enables precompiled headers with ccache
export CCACHE_SLOPPINESS=time_macros

# Determine the library path for GDAL
export GDAL_LIB_DIR=`gdal-config --libs | sed -e "s/-L//g" | sed -e "s/ *-lgdal.*//g"`
export GDAL_DATA=`gdal-config --datadir`

export LD_LIBRARY_PATH=$GDAL_LIB_DIR:$JAVA_HOME/jre/lib/amd64/server:$HOOT_HOME/lib:$LD_LIBRARY_PATH
export PATH=$HOOT_HOME/bin/:$PATH
export QT_SELECT=5

# If the user provided an argument then run as a command.
if [ ${#@} -gt 0 ]; then
    "$@"
fi
