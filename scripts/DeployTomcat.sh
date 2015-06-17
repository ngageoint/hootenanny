#!/bin/bash

set -e

function printOnError {
    OUT=/tmp/HootDeploy$RANDOM.log
    eval $* &> $OUT || (cat $OUT; rm -f $OUT; false)
    rm -f $OUT
}

function functionExists {
    type $1 2> /dev/null | grep -q "function"
}

if functionExists tomcatHome; then
    tomcatHome;
    export TOMCAT6_HOME=$result
fi

echo Tomcat home: $TOMCAT6_HOME

[ -d $TOMCAT6_HOME/webapps ] || (echo Please set TOMCAT6_HOME; exit -1)

cd $TOMCAT6_HOME
bin/catalina.sh stop &> /dev/null || true
rm -rf webapps/hoot-services/
cp $HOOT_HOME/hoot-services/target/hoot-services-*.war webapps/hoot-services.war
rm -rf webapps/hootenanny-id/
cp -R $HOOT_HOME/hoot-ui webapps/hootenanny-id
rm webapps/hootenanny-id/css/img
cp -R webapps/hootenanny-id/dist/img webapps/hootenanny-id/css/
sleep 10
# If the stop didn't work kill it hard
# Split tomcat6 so it doesn't match itself
pkill -9 -f $TOMCAT6_HOME"/endorsed" || true
printOnError bin/catalina.sh start
sleep 10
printOnError bin/catalina.sh stop -force 20
printOnError bin/catalina.sh start
sleep 20

