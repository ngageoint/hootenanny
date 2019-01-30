#!/bin/bash

HOOT_VERSION=`cat ../HOOT_VERSION_FILE | grep "HOOT_VERSION" | awk '{print $3;}'`
#echo "HOOT_VERSION: " $HOOT_VERSION
#echo "USER: " $USER

SERVICES_BUILD_INFO_FILE=./src/main/resources/build.info
echo "name=Hootenanny Web Services" > $SERVICES_BUILD_INFO_FILE
echo "version="$HOOT_VERSION >> $SERVICES_BUILD_INFO_FILE
echo "user="$USER >> $SERVICES_BUILD_INFO_FILE
  
# assume that if we're building services, we're also building iD, since iD doesn't have its
# own makefile
UI_BUILD_INFO_FILE=../hoot-ui/data/buildInfo.json
UI2X_BUILD_INFO_FILE=../hoot-ui-2x/data/buildInfo.json
echo "{ \"name\": \"Hootenanny iD Editor\", \"version\": \"$HOOT_VERSION\", \"user\": \"$USER\" }" > $UI_BUILD_INFO_FILE
echo "{ \"name\": \"Hootenanny iD Editor\", \"version\": \"$HOOT_VERSION\", \"user\": \"$USER\" }" > $UI2X_BUILD_INFO_FILE
