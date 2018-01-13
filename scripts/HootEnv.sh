#!/bin/bash

SCRIPT_FILE=`readlink -f ${BASH_SOURCE[0]}`
SCRIPT_PATH="$( cd "$( dirname "$SCRIPT_FILE" )" && pwd )"

export HOOT_HOME=`readlink -m $SCRIPT_PATH/../`
export HOOT_WORKING_NAME=`echo $HOOT_HOME | sed -e "s/.*\///g"`
export PRETTY_PIPES_HOME=$HOOT_HOME/pretty-pipes

# Enables precompiled headers with ccache
export CCACHE_SLOPPINESS=time_macros

# if the pretty pipes home isn't right
if [ ! -x $PRETTY_PIPES_HOME/bin/PrettyPipes ]; then
    unset PRETTY_PIPES_HOME
    # If the hoot home contains pretty pipes (typically happens when installed)
    if [ -x $HOOT_HOME/bin/PrettyPipes ]; then
        export PRETTY_PIPES_HOME=$HOOT_HOME/
    fi
fi

# Determine the library path for GDAL
export GDAL_LIB_DIR=`gdal-config --libs | sed -e "s/-L//g" | sed -e "s/ *-lgdal.*//g"`
export GDAL_DATA=`gdal-config --datadir`

export LD_LIBRARY_PATH=$GDAL_LIB_DIR:$HOOT_HOME/lib:$PRETTY_PIPES_HOME/lib/:$LD_LIBRARY_PATH
export PATH=$HOOT_HOME/bin/:$PATH
export QT_SELECT=4

# If the user provided an argument then run as a command.
if [ ${#@} -gt 0 ]; then
    "$@"
fi
