#!/usr/bin/env bash

export JDK_VERSION=1.8.0_152
export JDK_URL=http://download.oracle.com/otn-pub/java/jdk/8u152-b16/aa0333dd3019491ca4f6ddbe78cdb6d0/jdk-8u152-linux-x64.tar.gz
export JDK_TAR=jdk-8u152-linux-x64.tar.gz
export JDK_MD5=20dddd28ced3179685a5f58d3fcbecd8

export GDAL_VERSION=2.1.4

# FGDB 1.5 is required to compile using g++ >= 5.1
# https://trac.osgeo.org/gdal/wiki/FileGDB#HowtodealwithGCC5.1C11ABIonLinux
export FGDB_VERSION=1.5.1
export FGDB_VERSION2=`echo $FGDB_VERSION | sed 's/\./_/g;'`
export FGDB_TAR=FileGDB_API_1_5_64gcc51.tar.gz
export FGDB_URL=https://github.com/Esri/file-geodatabase-api/raw/master/FileGDB_API_1.5/$FGDB_FILE
