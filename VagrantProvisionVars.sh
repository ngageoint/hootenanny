#!/usr/bin/env bash

export JDK_VERSION=1.8.0_161
export JDK_URL=http://download.oracle.com/otn-pub/java/jdk/8u161-b12/2f38c3b165be4555a1fa6e98c45e0808/jdk-8u161-linux-x64.tar.gz
export JDK_TAR=jdk-8u161-linux-x64.tar.gz
export JDK_MD5=99051574a0d90871ed24a91a5d321ed2

# Hoot deps library versions
export GLPK_VERSION=4.64
export LIBOAUTHCPP_VERSION=0.1.0
export LIBPHONENUMBER_VERSION=8.12.27
export NODE_VERSION=14.16.1
export NPM_VERSION=6.14.12
export V8_VERSION=8.4.371.19
export STXXL_VERSION=1.3.1
export GOOGLE_CHROME_VERSION=91.0.4472.114

# Geoint deps library versions
export ARMADILLO_VERSION=8.600.1
export GDAL_VERSION=3.2.3
export GEOS_VERSION=3.9.2
export LIBGEOTIFF_VERSION=1.6.0
export PROJ_VERSION=7.2.1

# PostgreSQL version
export POSTGRESQL_VERSION=13
export POSTGRESQL_VERSION_DOTLESS="$(echo "$POSTGRESQL_VERSION" | awk '{ gsub(/\./, ""); print substr($0, 1, 2) }')"

# FGDB 1.5 is required to compile using g++ >= 5.1
# https://trac.osgeo.org/gdal/wiki/FileGDB#HowtodealwithGCC5.1C11ABIonLinux
export FGDB_VERSION=1.5.1
export FGDB_URL=https://github.com/Esri/file-geodatabase-api/raw/master/FileGDB_API_${FGDB_VERSION}/

# Devtoolset
export DEVTOOLSET_VERSION=8
