#!/usr/bin/env bash

set -e

###################################################
# VERY IMPORTANT: Set the $HOOT_HOME environment  #
# variable prior to running this script if ~/hoot #
# isn't the correct location for HOOT_HOME        #
###################################################
if [ -z "$HOOT_HOME" ]; then
    HOOT_HOME=~/hoot
fi
echo HOOT_HOME: $HOOT_HOME
#################################################

# Common set of file versions
source $HOOT_HOME/VagrantProvisionVars.sh

if ! rpm -qa | grep -q ^yum-plugin-versionlock ; then
    # Install the versionlock plugin version first.
    sudo yum install -y yum-plugin-versionlock >> CentOS_upgrade.txt 2>&1
elif [[ $(yum versionlock list | wc -l) -gt 2 ]] ; then  # versionlock list returns two lines if empty
    # Remove any version locks to allow upgrading when versions have changed.
    sudo yum versionlock delete \
         armadillo \
         geos \
         geos-devel \
         glpk \
         glpk-devel \
         gdal \
         gdal-devel \
         gdal-python-tools \
         google-chrome-stable \
         libgeotiff \
         libgeotiff-devel \
         liboauthcpp \
         liboauthcpp-devel \
         libphonenumber \
         libphonenumber-devel \
         nodejs \
         nodejs-devel \
         nodejs-docs \
         nodejs-libs \
         npm \
         proj \
         proj-devel \
         stxxl \
         stxxl-devel \
         v8-devel >> CentOS_upgrade.txt 2>&1
fi

echo "### Installing libraries with locked versions"
sudo yum install -y \
     armadillo-$ARMADILLO_VERSION \
     geos-$GEOS_VERSION \
     geos-devel-$GEOS_VERSION \
     glpk-$GLPK_VERSION \
     glpk-devel-$GLPK_VERSION \
     gdal-$GDAL_VERSION \
     gdal-devel-$GDAL_VERSION \
     gdal-python-tools-$GDAL_VERSION \
     google-chrome-stable-$GOOGLE_CHROME_VERSION \
     libgeotiff-$LIBGEOTIFF_VERSION \
     libgeotiff-devel-$LIBGEOTIFF_VERSION \
     libphonenumber-$LIBPHONENUMBER_VERSION \
     libphonenumber-devel-$LIBPHONENUMBER_VERSION \
     liboauthcpp-$LIBOAUTHCPP_VERSION \
     liboauthcpp-devel-$LIBOAUTHCPP_VERSION \
     npm-$NPM_VERSION \
     nodejs-$NODE_VERSION \
     nodejs-devel-$NODE_VERSION \
     nodejs-docs-$NODE_VERSION \
     nodejs-libs-$NODE_VERSION \
     proj-$PROJ_VERSION \
     proj-devel-$PROJ_VERSION \
     stxxl-$STXXL_VERSION \
     stxxl-devel-$STXXL_VERSION \
     v8-devel-$V8_VERSION

echo "### Locking versions of libraries"
sudo yum versionlock add \
     armadillo-$ARMADILLO_VERSION \
     geos-$GEOS_VERSION \
     geos-devel-$GEOS_VERSION \
     glpk-$GLPK_VERSION \
     glpk-devel-$GLPK_VERSION \
     gdal-$GDAL_VERSION \
     gdal-devel-$GDAL_VERSION \
     gdal-python-tools-$GDAL_VERSION \
     google-chrome-stable-$GOOGLE_CHROME_VERSION \
     libgeotiff-$LIBGEOTIFF_VERSION \
     libgeotiff-devel-$LIBGEOTIFF_VERSION \
     libphonenumber-$LIBPHONENUMBER_VERSION \
     libphonenumber-devel-$LIBPHONENUMBER_VERSION \
     liboauthcpp-$LIBOAUTHCPP_VERSION \
     liboauthcpp-devel-$LIBOAUTHCPP_VERSION \
     npm-$NPM_VERSION \
     nodejs-$NODE_VERSION \
     nodejs-devel-$NODE_VERSION \
     nodejs-docs-$NODE_VERSION \
     nodejs-libs-$NODE_VERSION \
     proj-$PROJ_VERSION \
     proj-devel-$PROJ_VERSION \
     stxxl-$STXXL_VERSION \
     stxxl-devel-$STXXL_VERSION \
     v8-devel-$V8_VERSION

# install useful and needed packages for working with hootenanny
echo "### Installing dependencies from repos..."
sudo yum -y install \
    asciidoc \
    autoconf \
    autoconf-archive \
    automake \
    bison \
    boost-devel \
    bzip2 \
    ccache \
    cmake \
    cppunit-devel \
    dblatex \
    devtoolset-$DEVTOOLSET_VERSION \
    devtoolset-$DEVTOOLSET_VERSION-libasan-devel \
    doxygen \
    gcc-c++ \
    git \
    git-core \
    gnuplot \
    lcov \
    libffi-devel \
    libicu-devel \
    libpng-devel \
    libtool \
    m4 \
    maven \
    mlocate \
    opencv \
    opencv-core \
    opencv-devel \
    opencv-python \
    osmosis \
    java-1.8.0-openjdk \
    perl-XML-LibXML \
    libpostal-data \
    libpostal-devel \
    parallel \
    postgresql${POSTGRESQL_VERSION_DOTLESS} \
    postgresql${POSTGRESQL_VERSION_DOTLESS}-contrib \
    postgresql${POSTGRESQL_VERSION_DOTLESS}-devel \
    postgresql${POSTGRESQL_VERSION_DOTLESS}-server \
    protobuf \
    protobuf-compiler \
    protobuf-devel \
    python \
    python-devel \
    python3 \
    python3-devel \
    python3-matplotlib \
    python3-pip \
    python3-setuptools \
    ruby \
    ruby-devel \
    rubygem-bundler \
    qt5-qtbase \
    qt5-qtbase-devel \
    qt5-qtbase-postgresql \
    qt5-qtwebkit \
    qt5-qtwebkit-devel \
    readline-devel \
    redhat-lsb-core \
    sqlite-devel \
    swig \
    tex-fonts-hebrew \
    texlive \
    texlive-collection-fontsrecommended \
    texlive-collection-langcyrillic \
    unzip \
    vim \
    wamerican-insane \
    w3m \
    wget \
    words \
    xorg-x11-server-Xvfb \
    zip
