#!/usr/bin/env bash

set -e

#####
# NOTE: This is a VERY cut down version of Hootenanny for a single purpose
#
# It runs on Centos7
#
# It DOES NOT include a database or a UI
#
# When I can sort out how to get systemd working in a non-privilaged container
# I will add Postgres and the UI back in.
#
# MattJ



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
source ~/.bash_profile

# add EPEL repo for extra packages
echo "### Add epel repo ###"
yum -y install epel-release
# add Hoot repo for our pre-built dependencies.
echo "### Add Hoot repo ###"
$HOOT_HOME/scripts/hoot-repo/yum-configure.sh

# add the Postgres repo
echo "### Add Postgres repo ###"
rpm -Uvh https://download.postgresql.org/pub/repos/yum/9.5/redhat/rhel-7-x86_64/pgdg-centos95-9.5-3.noarch.rpm  >> CentOS_upgrade.txt 2>&1

echo "Updating OS..."
echo "### Update ###"
yum -q -y update

# Not doing the next bit. The box is already up to date.
# echo "### Upgrade ###" >> CentOS_upgrade.txt
# yum -q -y upgrade >> CentOS_upgrade.txt 2>&1

# Make sure that we are in ~ before trying to wget & install stuff
cd ~

echo "### Installing the repo for an ancient version of NodeJS"
curl --silent --location https://rpm.nodesource.com/setup | bash -

echo "### Installing an ancient version of NodeJS"
yum install -y \
  nodejs-0.10.46 \
  nodejs-devel-0.10.46 \
  yum-plugin-versionlock

# Now try to lock NodeJS so that the next yum update doesn't remove it.
yum versionlock nodejs*


# install useful and needed packages for working with hootenanny
echo "### Installing dependencies from repos..."
yum -y install \
    autoconf \
    automake \
    bc \
    boost-devel \
    bzip2 \
    cmake \
    cppunit-devel \
    gcc \
    gcc-c++ \
    gdb \
    geos \
    geos-devel \
    git \
    git-core \
    glpk \
    glpk-devel \
    hoot-gdal \
    hoot-gdal-devel \
    hoot-gdal-python \
    libicu-devel \
    libpng-devel \
    libtool \
    mlocate \
    opencv \
    opencv-core \
    opencv-devel \
    opencv-python \
    osmosis \
    java-1.8.0-openjdk \
    perl-XML-LibXML \
    hoot-postgis23_95 \
    postgresql95 \
    postgresql95-contrib \
    postgresql95-devel \
    postgresql95-server \
    proj \
    proj-devel \
    protobuf \
    protobuf-compiler \
    protobuf-devel \
    python-devel \
    python-matplotlib \
    python-pip  \
    python-setuptools \
    qt \
    qt-devel \
    qt-postgresql \
    qtwebkit \
    qtwebkit-devel \
    redhat-lsb-core \
    stxxl \
    stxxl-devel \
    swig \
    unzip \
    v8-devel \
    vim \
    wamerican-insane \
    w3m \
    words \
    zip \


# Fix missing qmake
if ! hash qmake >/dev/null 2>&1 ; then
    if hash qmake-qt4 >/dev/null 2>&1 ; then
      alternatives --install /usr/bin/qmake qmake /usr/bin/qmake-qt4 500
    else
      echo "##### No qmake! #####"
    fi
fi

#####
# Temp change until we get the C++11 support into develop
cd $HOOT_HOME
cp LocalConfig.pri.orig LocalConfig.pri
echo "QMAKE_CXXFLAGS += -std=c++11" >> LocalConfig.pri
#####

# Not sure if we need this:
# if ! mocha --version &>/dev/null; then
#     echo "### Installing mocha for plugins test..."
#     npm install --silent -g mocha@3.5.3
#     # Clean up after the npm install
#     rm -rf ~/tmp
# fi

echo "SetupEnv.sh"
source ./SetupEnv.sh

echo "Building Hoot"
# Going to remove this so that it gets updated
if [ -f missing ]; then
  rm -f missing
fi

# We could just hardcode this to 9.5 but it is removed for the moment
# PG_VERSION=$(psql --version | egrep -o '[0-9]{1,}\.[0-9]{1,}')
#   --with-postgresql=/usr/pgsql-$PG_VERSION/bin/pg_config \

aclocal && autoconf && autoheader && automake --add-missing --copy

./configure \
  --with-rnd \
  --without-services \
  --without-postgresql \
  --prefix=/usr/ \
  --datarootdir=/usr/share/hootenanny/ \
  --docdir=/usr/share/doc/hootenanny/ \
  --localstatedir=/var/lib/hootenanny/ \
  --libdir=/usr/lib64 \
  --sysconfdir=/etc/


echo "Building Hoot... "
make -s clean && make -sj$(nproc)

# This stops the install dieing if it cant copy PDF files.
touch docs/empty.pdf

echo "Installing Hoot"
make -s install

# Get Hoot into the default path
echo "export HOOT_HOME=/var/lib/hootenanny" > /etc/profile.d/hootenanny.sh

chmod 755 /etc/profile.d/hootenanny.sh
cp -R test-files /var/lib/hootenanny/
ln -s /usr/lib64 /var/lib/hootenanny/lib
rm -f /usr/bin/HootEnv.sh

# This allows all the tests to run.
mkdir -p /var/lib/hootenanny/hoot-core-test/src/test/
ln -s /var/lib/hootenanny/test-files/ /var/lib/hootenanny/hoot-core-test/src/test/resources

# This makes it so HootEnv.sh resolves hoot home properly.
ln -s /var/lib/hootenanny/bin/HootEnv.sh /usr/bin/HootEnv.sh


## Cleanup stuff that we don't need in the final image
yum remove -y \
  autoconf \
  automake \
  boost-devel\
  cmake \
  gcc \
  gcc-c++ \
  geos-devel \
  glpk-devel \
  java-1.8.0-openjdk \
  opencv-devel \
  perl-XML-LibXML \
  postgresql95 \
  postgresql95-devel \
  postgresql95-server \
  proj-devel \
  protobuf-devel \
  python-devel \
  python-matplotlib \
  qt-devel \
  qtwebkit \
  qtwebkit-devel \
  stxxl-devel \
  v8-devel \
  vim

yum -y autoremove

# Now put some stuff back that got whacked by autoremove
yum install -y \
  boost-iostreams \
  boost-system \
  cppunit \
  qt-x11 \
  v8

yum clean all
rm -rf /var/cache/yum
##########################################