#!/bin/bash
set -e

BASEDIR=$(dirname $0)
if [ -f $BASEDIR/../Config.sh ]; then
  source $BASEDIR/../Config.sh
else
  source $HOOT_HOME/conf/Config.sh
fi

export CATALINA_BASE=$TOMCAT6_HOME
export CATALINA_HOME=$TOMCAT6_HOME

echo Tomcat home: $TOMCAT6_HOME

[ -d $TOMCAT6_HOME/webapps ] || (echo Please set TOMCAT6_HOME; exit -1)

cd $TOMCAT6_HOME
bin/shutdown.sh || true
sleep 10
# If the stop didn't work kill it hard
pkill -9 -f $TOMCAT6_HOME"/endorsed" || true

