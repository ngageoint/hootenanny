#!/bin/bash

HOOT_VERSION=`cat ../HOOT_VERSION_FILE | grep "HOOT_VERSION" | awk '{print $3;}'`
#echo "HOOT_VERSION: " $HOOT_VERSION
#echo "USER: " $USER
HOOT_VERSION_DATE=`cat ../HOOT_VERSION_FILE | grep "HOOT_DATE" | awk '{print $3;}'`
HOOT_BUILT_BY=`cat ../HOOT_VERSION_FILE | grep "HOOT_BUILT_BY" | awk '{print $3;}'`

SERVICES_BUILD_INFO_FILE=./src/main/resources/build.info
echo "name=Services" > $SERVICES_BUILD_INFO_FILE
echo "version="$HOOT_VERSION >> $SERVICES_BUILD_INFO_FILE
echo "date="$HOOT_VERSION_DATE >> $SERVICES_BUILD_INFO_FILE
echo "user="$HOOT_BUILT_BY >> $SERVICES_BUILD_INFO_FILE
