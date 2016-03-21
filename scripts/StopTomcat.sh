#!/bin/bash
set -e

BASEDIR=$(dirname $0)
if [ -f $BASEDIR/../Config.sh ]; then
  # We're running tests in the CI environment.
  source $BASEDIR/../Config.sh
else
  # We're running tests in a development environment.
  source $HOOT_HOME/conf/Config.sh
fi

echo Stopping Tomcat...

export CATALINA_BASE=$TOMCAT6_HOME
export CATALINA_HOME=$TOMCAT6_HOME

echo Tomcat home: $TOMCAT6_HOME

[ -d $TOMCAT6_HOME/webapps ] || (echo Please set TOMCAT6_HOME; exit -1)
  
$TOMCAT6_HOME/bin/shutdown.sh || true
sleep 10
# If the stop didn't work, kill it hard.
echo Forcing Tomcat shutdown...
pkill -9 -f $TOMCAT6_HOME"/endorsed" || true 

echo Tomcat stopped.

