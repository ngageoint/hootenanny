#!/usr/bin/env bash

# Java version - EL9 ships with Java 11 and 17
export JDK_VERSION=11

# Hoot deps library versions
export GLPK_VERSION=4.65
export LIBOAUTHCPP_VERSION=0.1.0
export LIBPHONENUMBER_VERSION=8.13.27
export LIBPOSTAL_VERSION=1.1
export NODE_VERSION=18.19.0
export NPM_VERSION=10.2.3
export V8_VERSION=10.2.154.26
export STXXL_VERSION=1.4.1
export GOOGLE_CHROME_VERSION=120.0.6099.109

# Geoint deps library versions
export ARMADILLO_VERSION=12.6.4
export GDAL_VERSION=3.8.3
export GEOS_VERSION=3.12.1
export LIBGEOTIFF_VERSION=1.7.1
export PROJ_VERSION=9.3.1

# PostgreSQL version - EL9 supports PostgreSQL 13-16
export POSTGRESQL_VERSION=15
export POSTGRESQL_VERSION_DOTLESS="$(echo "$POSTGRESQL_VERSION" | awk '{ gsub(/\./, ""); print substr($0, 1, 2) }')"

# Devtoolset - NOT NEEDED on EL9 (GCC 11.x is default)
# Keeping variable for backward compatibility in scripts
export DEVTOOLSET_VERSION=none
