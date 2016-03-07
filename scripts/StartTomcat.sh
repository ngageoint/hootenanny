#!/bin/bash
set -e

BASEDIR=$(dirname $0)
source $BASEDIR/../Config.sh

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

cd $TOMCAT6_HOME
#printOnError bin/catalina.sh start
bin/startup.sh

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
