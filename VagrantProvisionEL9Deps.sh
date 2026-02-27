#!/usr/bin/env bash

set -e

# Get the directory of this script
SCRIPT_HOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Common set of file versions
source $SCRIPT_HOME/VagrantProvisionVars.sh

echo "### Installing Hootenanny dependencies for EL9 ###"

# Remove any version locks if they exist (from previous runs)
if rpm -qa | grep -q ^dnf-plugin-versionlock ; then
    if [[ $(dnf versionlock list | wc -l) -gt 0 ]] ; then
        echo "### Clearing existing version locks ###"
        sudo dnf versionlock clear
    fi
fi

# Install the versionlock plugin if not present
if ! rpm -qa | grep -q ^dnf-plugin-versionlock ; then
    sudo dnf install -y dnf-plugin-versionlock >> EL9_upgrade.txt 2>&1
fi

echo "### Installing libraries with locked versions"
sudo dnf install -y \
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
     liboauthcpp-$LIBOAUTHCPP_VERSION \
     liboauthcpp-devel-$LIBOAUTHCPP_VERSION \
     libphonenumber-$LIBPHONENUMBER_VERSION \
     libphonenumber-devel-$LIBPHONENUMBER_VERSION \
     libpostal-$LIBPOSTAL_VERSION \
     libpostal-data-$LIBPOSTAL_VERSION \
     libpostal-devel-$LIBPOSTAL_VERSION \
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
sudo dnf versionlock add \
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
     liboauthcpp-$LIBOAUTHCPP_VERSION \
     liboauthcpp-devel-$LIBOAUTHCPP_VERSION \
     libphonenumber-$LIBPHONENUMBER_VERSION \
     libphonenumber-devel-$LIBPHONENUMBER_VERSION \
     libpostal-$LIBPOSTAL_VERSION \
     libpostal-data-$LIBPOSTAL_VERSION \
     libpostal-devel-$LIBPOSTAL_VERSION \
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

# Install useful and needed packages for working with hootenanny
# Note: EL9 differences from EL7:
# - No devtoolset needed (GCC 11 is default)
# - python/python-devel removed (Python 2 is gone)
# - redhat-lsb-core deprecated, use redhat-lsb instead
echo "### Installing dependencies from repos..."
sudo dnf -y install \
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
    osmosis \
    java-${JDK_VERSION}-openjdk \
    java-${JDK_VERSION}-openjdk-devel \
    perl-XML-LibXML \
    parallel \
    postgresql${POSTGRESQL_VERSION_DOTLESS} \
    postgresql${POSTGRESQL_VERSION_DOTLESS}-contrib \
    postgresql${POSTGRESQL_VERSION_DOTLESS}-devel \
    postgresql${POSTGRESQL_VERSION_DOTLESS}-server \
    protobuf \
    protobuf-compiler \
    protobuf-devel \
    python3 \
    python3-devel \
    python3-matplotlib \
    python3-pip \
    python3-setuptools \
    qt5-qtbase \
    qt5-qtbase-devel \
    qt5-qtbase-postgresql \
    qt5-qtwebkit \
    qt5-qtwebkit-devel \
    readline-devel \
    sqlite-devel \
    swig \
    texlive \
    texlive-collection-fontsrecommended \
    texlive-collection-langcyrillic \
    unzip \
    vim \
    w3m \
    wget \
    words \
    xorg-x11-server-Xvfb \
    zip

echo "### Installing Python packages ###"
sudo pip3 install --upgrade pip
sudo pip3 install numpy scipy matplotlib

echo "### EL9 dependencies installation complete ###"

