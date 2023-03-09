#!/bin/bash

HOOT_VERSION=`cat ../HOOT_VERSION_FILE | grep "HOOT_VERSION" | awk '{print $3;}'`
#echo "HOOT_VERSION: " $HOOT_VERSION
#echo "USER: " $USER

SERVICES_BUILD_INFO_FILE=./src/main/resources/build.info
echo "name=Hootenanny Web Services" > $SERVICES_BUILD_INFO_FILE
echo "version="$HOOT_VERSION >> $SERVICES_BUILD_INFO_FILE
echo "user="$USER >> $SERVICES_BUILD_INFO_FILE
