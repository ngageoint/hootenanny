#!/bin/bash

set -e

BASEDIR=$(dirname $0)
if [ -f $BASEDIR/../Config.sh ]; then
  # We're running tests in the CI environment.
  source $BASEDIR/../Config.sh
else
  # We're running tests in a development environment.
  source $HOOT_HOME/conf/ServerConfig.sh
fi

function printOnError {
    OUT=/tmp/HootDeploy$RANDOM.log
    eval $* &> $OUT || (cat $OUT; rm -f $OUT; false)
    rm -f $OUT
}

[ -d $TOMCAT6_HOME/webapps ] || (echo Please set TOMCAT6_HOME; exit -1)

if [ "$1" == "--quiet" ]; then
  $HOOT_HOME/scripts/StopTomcat.sh > /dev/null
else
  $HOOT_HOME/scripts/StopTomcat.sh
fi

if [ "$1" == "--quiet" ]; then
  $HOOT_HOME/scripts/CopyWebAppsToTomcat.sh > /dev/null
else
  $HOOT_HOME/scripts/CopyWebAppsToTomcat.sh
fi

if [ "$1" == "--quiet" ]; then
  $HOOT_HOME/scripts/StartTomcat.sh > /dev/null
else
  $HOOT_HOME/scripts/StartTomcat.sh
fi

