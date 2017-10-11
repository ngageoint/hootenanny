#!/usr/bin/env bash

export JDK_FILE=jdk-8u131-linux-x64.tar.gz
export JDK_URL=http://download.oracle.com/otn-pub/java/jdk/8u131-b11/d54c1d3a095b4ff2b6607d096fa80163/$JDK_FILE
export JDK_DIR=jdk1.8.0_131
export JDK_MD5=75b2cb2249710d822a60f83e28860053

export GDAL_VERSION=2.1.4
# FGDB 1.5 is required to compile using g++ >= 5.1
# https://trac.osgeo.org/gdal/wiki/FileGDB#HowtodealwithGCC5.1C11ABIonLinux
export FGDB_VERSION=1.5.1
export FGDB_VERSION2=`echo $FGDB_VERSION | sed 's/\./_/g;'`
export FGDB_FILE=FileGDB_API_1_5_64gcc51.tar.gz
export FGDB_URL=https://github.com/Esri/file-geodatabase-api/raw/master/FileGDB_API_1.5/$FGDB_FILE
