#!/usr/bin/env bash

export JDK_VERSION=1.8.0

# Hoot deps library versions
export GLPK_VERSION=4.64
export LIBOAUTHCPP_VERSION=0.1.0
export LIBPHONENUMBER_VERSION=8.12.39
export LIBPOSTAL_VERSION=1.1
export NODE_VERSION=14.16.1
export NPM_VERSION=6.14.12
export V8_VERSION=8.4.371.19
export STXXL_VERSION=1.3.1
export GOOGLE_CHROME_VERSION=91.0.4472.114

# Geoint deps library versions
export ARMADILLO_VERSION=10.8.2
export GDAL_VERSION=3.2.3
export GEOS_VERSION=3.9.3
export LIBGEOTIFF_VERSION=1.6.0
export PROJ_VERSION=7.2.1

# PostgreSQL version
export POSTGRESQL_VERSION=13
export POSTGRESQL_VERSION_DOTLESS="$(echo "$POSTGRESQL_VERSION" | awk '{ gsub(/\./, ""); print substr($0, 1, 2) }')"

# Devtoolset
export DEVTOOLSET_VERSION=8
