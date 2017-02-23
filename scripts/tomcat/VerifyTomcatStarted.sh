#!/bin/bash
set -e

echo "Checking Tomcat server for Hootenanny availability..."

TOMCAT_PORT=$1
COUNT=0
VERSION_INFO="`curl --silent --show-error --connect-timeout 1 http://localhost:$TOMCAT_PORT/hoot-services/info/about/coreVersionInfo | grep 'Hootenanny Core' || true`"
until [ "$VERSION_INFO" != "" ]; do
  sleep 3
  VERSION_INFO="`curl --silent --show-error --connect-timeout 1 http://localhost:$TOMCAT_PORT/hoot-services/info/about/coreVersionInfo | grep 'Hootenanny Core' || true`"
  let COUNT=COUNT+1
  [ $COUNT -gt 20 ] && (echo Tomcat not started or Hootenanny web application not deployed to http://localhost:$TOMCAT_PORT.; exit -1)
done 
	
echo "Tomcat is started."

exit 0
