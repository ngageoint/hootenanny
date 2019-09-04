#!/usr/bin/env bash

export JDK_VERSION=1.8.0_161
export JDK_URL=http://download.oracle.com/otn-pub/java/jdk/8u161-b12/2f38c3b165be4555a1fa6e98c45e0808/jdk-8u161-linux-x64.tar.gz
export JDK_TAR=jdk-8u161-linux-x64.tar.gz
export JDK_MD5=99051574a0d90871ed24a91a5d321ed2

# Dependency library versions.
export ARMADILLO_VERSION=8.300.0
export GDAL_VERSION=2.1.4
export GEOS_VERSION=3.6.2
export GLPK_VERSION=4.64
export LIBOAUTHCPP_VERSION=0.1.0
export LIBGEOTIFF_VERSION=1.4.2
export LIBPHONENUMBER_VERSION=8.9.16
export NODE_VERSION=8.9.3
export PROJ_VERSION=4.8.0
export STXXL_VERSION=1.3.1

# FGDB 1.5 is required to compile using g++ >= 5.1
# https://trac.osgeo.org/gdal/wiki/FileGDB#HowtodealwithGCC5.1C11ABIonLinux
export FGDB_VERSION=1.5.1
export FGDB_URL=https://github.com/Esri/file-geodatabase-api/raw/master/FileGDB_API_${FGDB_VERSION}/

# Ruby versions and locations.
export BUNDLER_VERSION=2.0.1
export RUBY_BASE_URL=https://s3.amazonaws.com/hoot-ruby
export RUBY_GEMS_URL="${RUBY_BASE_URL}/gems"
export RUBY_VERSION_HOOT=2.3.7
export RVM_BASE_URL=https://github.com/rvm/rvm
export RVM_BINARIES_URL="${RUBY_BASE_URL}/binaries"
export RVM_HOME="${RVM_HOME:-${HOME}/.rvm}"
export RVM_VERSION=1.29.4
