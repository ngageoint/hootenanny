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

function printOnError {
    OUT=/tmp/HootDeploy$RANDOM.log
    eval $* &> $OUT || (cat $OUT; rm -f $OUT; false)
    rm -f $OUT
}

echo Tomcat home: $TOMCAT6_HOME
export CATALINA_BASE=$TOMCAT6_HOME
export CATALINA_HOME=$TOMCAT6_HOME
hoot version &> /dev/null || (echo Please make sure hoot is in your PATH.; exit -1)

[ -d $TOMCAT6_HOME/webapps ] || (echo Please set TOMCAT6_HOME; exit -1)

echo Starting Tomcat.  Making connection attempts... 

#printOnError bin/catalina.sh start
$TOMCAT6_HOME/bin/startup.sh

# Wait for tomcat to start
COUNT=0
VERSION_INFO="`curl --silent --show-error --connect-timeout 1 http://localhost:$TOMCAT_PORT/hoot-services/info/about/coreVersionInfo | grep 'Hootenanny Core' || true`"
until [ "$VERSION_INFO" != "" ]; do
  sleep 3
  VERSION_INFO="`curl --silent --show-error --connect-timeout 1 http://localhost:$TOMCAT_PORT/hoot-services/info/about/coreVersionInfo | grep 'Hootenanny Core' || true`"
  let COUNT=COUNT+1
  [ $COUNT -gt 20 ] && (echo Tomcat didn\'t start; exit -1)
done

exit 0
