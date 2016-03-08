#!/bin/bash
set -e

BASEDIR=$(dirname $0)
if [ -f $BASEDIR/../Config.sh ]; then
  source $BASEDIR/../Config.sh
else
  source $HOOT_HOME/conf/Config.sh
fi

function printOnError {
    OUT=/tmp/HootDeploy$RANDOM.log
    eval $* &> $OUT || (cat $OUT; rm -f $OUT; false)
    rm -f $OUT
}

echo Tomcat home: $TOMCAT6_HOME
#echo test5
export CATALINA_BASE=$TOMCAT6_HOME
export CATALINA_HOME=$TOMCAT6_HOME
hoot version &> /dev/null || (echo Please make sure hoot is in your PATH.; exit -1)

#echo test2

[ -d $TOMCAT6_HOME/webapps ] || (echo Please set TOMCAT6_HOME; exit -1)

echo Starting Tomcat.  Making connection attempts... 

cd $TOMCAT6_HOME
#printOnError bin/catalina.sh start
bin/startup.sh

#echo test3

# Wait for tomcat to start
COUNT=0
VERSION_INFO="`curl --silent --show-error --connect-timeout 1 http://localhost:$TOMCAT_PORT/hoot-services/info/about/coreVersionInfo | grep 'Hootenanny Core' || true`"
until [ "$VERSION_INFO" != "" ]; do
  sleep 3
  VERSION_INFO="`curl --silent --show-error --connect-timeout 1 http://localhost:$TOMCAT_PORT/hoot-services/info/about/coreVersionInfo | grep 'Hootenanny Core' || true`"
  let COUNT=COUNT+1
  [ $COUNT -gt 20 ] && (echo Tomcat didn\'t start; exit -1)
done

#echo test4

exit 0
