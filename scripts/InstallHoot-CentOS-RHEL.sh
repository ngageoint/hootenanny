#!/bin/bash

# Execute this script something like this:
#
## HOOT_VERSION is of the form X.Y.Z and corresponds to a release available at 
## http://github.com/ngageoint/hootenanny.
#
#HOOT_VERSION=X.Y.Z
#
#cd ~
#wget https://github.com/ngageoint/hootenanny/releases/download/v$HOOT_VERSION/hootenanny-$HOOT_VERSION.tar.gz
#tar -xzf hootenanny-$HOOT_VERSION.tar.gz
#cp hootenanny-$HOOT_VERSION.tar.gz/scripts/InstallHoot-CentOS-RHEL.sh .

## Follow the instructions to configure the installation script.  You may configure the script to 
## install all Hootenanny dependencies, as well as the application or have it just install the 
## Hootenanny application.  SET HOOT VERSION INSIDE THIS SCRIPT The dependencies must be installed 
# at least once to the target environment.  Then:
#
#./InstallHoot-CentOS-RHEL.sh
#
# This script: 
# - applies to the production CentOS 6.x environment only (tested on CentOS 6.6)
# - retains all existing hoot settings (basemaps, translations, etc.)
# - *does not* back out of the entire installation process in all cases if errors occur during the 
# installation
# - is not bulletproof and is meant as a placeholder until a Hootenanny CentOS Vagrant installation
# is available
# - *does not* currently install Hadoop
#
# WARNING: Spend time to reading the comments to understand what this script is doing before 
# attempting to execute it.  USE THIS SCRIPT AT YOUR OWN RISK.

# VARS IN THIS SECTION NEED TO BE CUSTOMIZED FOR EACH INSTALL

# Dependencies install needs to be set to true the first time you install Hootenanny *only*.
INSTALL_DEPENDENCIES=false
INSTALL_HOOT_APPLICATION=false
# name of the version you're deploying; version format is X.Y.Z; this should match the version of
# the archive files being deployed
HOOT_VERSION=0.2.17
# the user group used when installing hoot core; needed for compiling hoot core only; The user 
# executing this script must be a member of this group, and the group must have the correct 
# privileges to install files to HOOT_CORE_INSTALL_DIR.
CORE_INSTALL_USER_GROUP=root
# Only enable this if your environment is configured to use the FOUO translations.
UPDATE_FOUO_TRANSLATIONS=false
# This has to do with some silliness, where despite setting a virtualbox shared folder not to be
# read-only, the dependencies install still has problems writing to some files it needs to in
# the source install dir...a permissions issue that needs sorting out...so setting this to true
# is a workaround.
INSTALLING_FROM_VIRTUALBOX_SHARED_FOLDER=false

# VARS IN THIS SECTION DON'T NORMALLY NEED TO BE CHANGED

# Where the Hootenanny core application is installed to; usually: /usr/local
HOOT_CORE_INSTALL_DIR=/usr/local
# Be SUPER CERTAIN about enabling this one...all your data in the hoot database will be cleared; 
# won't run unless UPDATE_DATABASE is also enabled
CLEAR_DATABASE=false
# CORE_RUNTIME_USER and CORE_RUNTIME_USER_GROUP are the group permissions used for hoot at 
# runtime, which may differ from what was used at install time.  Do not change this unless you are
# not installing the web services or web application parts of hoot.
CORE_RUNTIME_USER=tomcat
CORE_RUNTIME_USER_GROUP=tomcat
# used for storing the archives to be installed, backups, and temporary file storage
# TODO: This setting has issues if its not set to ~ for some reason...
TEMP_DIR=~
TOMCAT_HOME=/var/lib/tomcat6/webapps
DEEGREE_DIR_ROOT=/usr/share/tomcat6
# relative to home dir; leave blank for no backups
BACKUP_DIR_NAME=Backup
DATE=`date +%m-%d-%Y`
# Add port 8080 to this URL if port forwarding to port 80 is not to be activated for some reason.
BASE_SERVICES_URL="http://localhost"
# TODO: This really should be read from the hoot-services.conf ingestStagingPath setting
BASEMAP_HOME=$TOMCAT_HOME/hoot-services/ingest/upload
ASCIIDOC_FILTERS_HOME=/usr/local/etc/asciidoc/filters
# Only set this to false if debugging the hoot core portion of the installation.
REMOVE_EXISTING_CORE_INSTALL=true
# Use this for adding optional functionality to hoot core at compile time; e.g. "--with-rnd"
CORE_CONFIGURATION_OPTIONS="--with-rnd"
# These should always be set to true, unless debugging or not installing the hoot web application.
UPDATE_DATABASE=true
UPDATE_CORE=true
UPDATE_SERVICES=true
UPDATE_UI=true
SILENT_INSTALL=false

if [ "$INSTALL_DEPENDENCIES" == "" ]; then
  echo "Please enter the choice for INSTALL_DEPENDENCIES."
  exit -1
fi
if [ "$INSTALL_HOOT_APPLICATION" == "" ]; then
  echo "Please enter the choice for INSTALL_HOOT_APPLICATION."
  exit -1
fi

if [ "$SILENT_INSTALL" == "false" ]; then
  if [ "$INSTALL_DEPENDENCIES" == "false" ]; then
    if [ "$INSTALL_HOOT_APPLICATION" == "false" ]; then
      echo "This script has not been configured.  Select either dependency installation, application installation, or both.  Installation canceled."
      exit -1;
    fi
  fi
  if [ "$INSTALL_DEPENDENCIES" == "true" ]; then
    echo "You're installing Hootenanny dependencies."
  fi
  if [ "$INSTALL_HOOT_APPLICATION" == "true" ]; then
    echo "You're installing the Hootenanny application version $HOOT_VERSION."
  fi
  echo "Is the above information correct?  y/n?"
  read clear
  if [ "$clear" == "n" ]; then
    exit -1
  fi
fi

INSTALL_SOURCE_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [ "$INSTALL_DEPENDENCIES" == "true" ]; then

  # Hootenanny on CentOS is fairly picky about which package versions will work with the software 
  # correctly, hence the more complicated dependency instructions found in this section.

  # remove old gnuplot
  sudo yum erase gnuplot gnuplot-latex gnuplot-common
  
  # Install everything from the yum repo where hoot is ok with using the latest version.
  echo "Installing packages from yum..."
  sudo yum -y install gcc gcc-c++ libicu-devel boost-devel python-devel gdb cmake python-numeric swig glpk glpk-devel python-argparse apr-devel apr-util-devel nodejs-devel nodejs v8-devel libX11-devel fontconfig-devel libXcursor-devel libXext-devel libXfixes libXft-devel libXi-devel libXrandr-devel libXrender-devel java-1.7.0-openjdk-devel unixODBC-devel.x86_64 uuid.x86_64 tomcat6 tomcat6-webapps tomcat6-admin-webapps freetype-devel   wget autoconf libxslt poppler kpathsea xdvipdfmx dvipdfm dvipng teckit perl-PDF-Reuse ruby tex-preview python-requests geos hdf5 json-c proj gdal npm unzip doxygen nano gd-devel libX11-devel libXau-devel libXpm-devel libjpeg-turbo-devel libxcb-devel xorg-x11-proto-devel words
  sudo yum -y install python-argparse || export ARGPARSE_FAILED=1
  if [ "$ARGPARSE_FAILED" == "1" ]; then
    sudo yum -y install python-setuptools
    easy_install argparse
  fi

  # Install everything where the exact version is important.  In some cases, section where a package is being built from source may be able to be replaced with a downloaded RPM.
  
  test -f epel-release-6-8.noarch.rpm || wget http://download.fedoraproject.org/pub/epel/6/x86_64/epel-release-6-8.noarch.rpm
  sudo rpm -ivh epel-release-6-8.noarch.rpm

  echo "Installing CPPUnit..."
  rm -rf cppunit-1.12.1
  test -f cppunit-1.12.1.tar.gz || wget http://sourceforge.net/projects/cppunit/files/cppunit/1.12.1/cppunit-1.12.1.tar.gz/download -O cppunit-1.12.1.tar.gz
  tar -xzf cppunit-1.12.1.tar.gz
  cd cppunit-1.12.1
  ./configure -q --prefix=$HOOT_CORE_INSTALL_DIR/ && make -j8 && sudo make install
  cd ..

  echo "Installing proj4j..."
  rm -rf proj-4.8.0
  test -f proj-4.8.0.tar.gz || wget http://download.osgeo.org/proj/proj-4.8.0.tar.gz
  tar xzf proj-4.8.0.tar.gz
  cd proj-4.8.0
  ./configure -q --prefix=$HOOT_CORE_INSTALL_DIR && make -j8 && sudo make install
  cd ..

  echo "Installing GEOS..."
  rm -rf geos-3.3.8
  test -f geos-3.3.8-patched.tgz || wget https://github.com/ngageoint/hootenanny/releases/download/v0.2.16/geos-3.3.8-patched.tgz
  tar -xzf geos-3.3.8-patched.tgz
  cd geos-3.3.8
  ./configure -q --prefix=$HOOT_CORE_INSTALL_DIR && make -j8 && sudo make install
  cd ..

  echo "Installing OpenCV..."
  rm -rf opencv-2.4.5
  tar xzf opencv-2.4.5.tar.gz
  if [ "$INSTALLING_FROM_VIRTUALBOX_SHARED_FOLDER" == "true" ]; then
    # for whatever reason you can't install this with cmake from a virtualbox shared folder
    rm -rf $HOOT_CORE_INSTALL_DIR/opencv-2.4.5
    cp -R opencv-2.4.5 $HOOT_CORE_INSTALL_DIR
    cd $HOOT_CORE_INSTALL_DIR/opencv-2.4.5
  else
    cd opencv-2.4.5
  fi
  mkdir -p release
  cd release
  cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=$HOOT_CORE_INSTALL_DIR -D BUILD_PYTHON_SUPPORT=OFF -D BUILD_EXAMPLES=OFF -D WITH_CUDA=OFF ..
  make && sudo make install
  if [ "$INSTALLING_FROM_VIRTUALBOX_SHARED_FOLDER" == "true" ]; then
    cd $INSTALL_SOURCE_DIR
  else
    cd ../..
  fi

  echo "Installing Protocol Buffers..."
  rm -rf protobuf-2.4.1
  test -f protobuf-2.4.1.tar.bz2 || wget http://protobuf.googlecode.com/files/protobuf-2.4.1.tar.bz2
  tar -xjf protobuf-2.4.1.tar.bz2
  cd protobuf-2.4.1
  ./configure -q --prefix=$HOOT_CORE_INSTALL_DIR && make -j8 && sudo make install
  cd ..

  # Optional
  # echo "Installing log4cxx..."
  # You need to patch Log4cxx to get it working. See https://issues.apache.org/jira/browse/LOGCXX-360
  #test -f apache-log4cxx-0.10.0.tar.gz || wget http://apache.spinellicreations.com/logging/log4cxx/0.10.0/apache-log4cxx-0.10.0.tar.gz
  #tar -xzf apache-log4cxx-0.10.0.tar.gz
  #cd apache-log4cxx-0.10.0
  #./configure --prefix=$HOOT_CORE_INSTALL_DIR && make -j8 && sudo make install
  #cd ..

  # Optional
  # echo "Installing stxxl..."
  #rm -rf stxxl-1.3.1
  #test -f stxxl-1.3.1.tar.gz || wget http://downloads.sourceforge.net/project/stxxl/stxxl/1.3.1/stxxl-1.3.1.tar.gz
  #tar -xzf stxxl-1.3.1.tar.gz
  #cd stxxl-1.3.1
  #make config_gnu || echo "Ignore the error"
  #ENABLED_SHARED=yes ; make library_g++
  #sudo cp lib/libstxxl.* $HOOT_CORE_INSTALL_DIR/lib/
  #sudo cp -R include/* $HOOT_CORE_INSTALL_DIR/include/
  #cd ..

  echo "Installing reporting packages..."

  test -f blas-3.2.1-4.el6.x86_64.rpm || wget http://www.my-guides.net/en/images/stories/fedora12/msttcore-fonts-2.0-3.noarch.rpm
  sudo rpm -Uvh msttcore-fonts-2.0-3.noarch.rpm
  if ! grep --quiet GDFONTPATH /etc/profile; then
    echo 'export GDFONTPATH=/usr/share/fonts/msttcore' >> /etc/profile
    source /etc/profile
  fi

  test -f blas-3.2.1-4.el6.x86_64.rpm || wget ftp://rpmfind.net/linux/centos/6.6/os/x86_64/Packages/blas-3.2.1-4.el6.x86_64.rpm
  test -f lapack-3.2.1-4.el6.x86_64.rpm || wget ftp://rpmfind.net/linux/centos/6.6/os/x86_64/Packages/lapack-3.2.1-4.el6.x86_64.rpm
  sudo rpm -ivh blas-3.2.1-4.el6.x86_64.rpm
  sudo rpm -ivh lapack-3.2.1-4.el6.x86_64.rpm

  test -f gnuplot-common-4.2.6-2.el6.x86_64.rpm || wget ftp://mirror.switch.ch/pool/4/mirror/scientificlinux/6rolling/x86_64/os/Packages/gnuplot-common-4.2.6-2.el6.x86_64.rpm
  test -f gnuplot-4.2.6-2.el6.x86_64.rpm || wget http://pkgs.org/centos-6/centos-x86_64/gnuplot-4.2.6-2.el6.x86_64.rpm/download/ -O gnuplot-4.2.6-2.el6.x86_64.rpm
  test -f gnuplot-latex-4.2.6-2.el6.noarch.rpm || wget http://mirror.centos.org/centos/6/os/i386/Packages/gnuplot-latex-4.2.6-2.el6.noarch.rpm
  sudo rpm -ivh gnuplot-common-4.2.6-2.el6.x86_64.rpm
  sudo rpm -ivh gnuplot-4.2.6-2.el6.x86_64.rpm
  sudo rpm -ivh gnuplot-latex-4.2.6-2.el6.noarch.rpm

  test -f gnuplot-4.6.4.tar.gz || wget http://sourceforge.net/projects/gnuplot/files/gnuplot/4.6.4/gnuplot-4.6.4.tar.gz/download -O gnuplot-4.6.4.tar.gz
  tar -zxf gnuplot-4.6.4.tar.gz
  cd gnuplot-4.6.4
  ./configure --prefix=/usr/local && make && sudo make install

  test -f libpng-1.2.49-1.el6_2.x86_64.rpm || wget http://mirror.centos.org/centos/6/os/x86_64/Packages/libpng-1.2.49-1.el6_2.x86_64.rpm
  test -f libpng-devel-1.2.49-1.el6_2.x86_64.rpm || wget http://mirror.centos.org/centos/6/os/x86_64/Packages/libpng-devel-1.2.49-1.el6_2.x86_64.rpm
  sudo rpm -ivh libpng-1.2.49-1.el6_2.x86_64.rpm
  sudo rpm -ivh libpng-devel-1.2.49-1.el6_2.x86_64.rpm

  #TODO: There are some redundant texlive libraries being installed here
  test -f texlive-2007-57.el6_2.x86_64.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-2007-57.el6_2.x86_64.rpm
  test -f texlive-texmf-2007-38.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-2007-38.el6.noarch.rpm
  test -f texlive-texmf-errata-2007-7.1.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-errata-2007-7.1.el6.noarch.rpm
  test -f texlive-texmf-fonts-2007-38.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-fonts-2007-38.el6.noarch.rpm
  test -f texlive-afm-2007-57.el6_2.x86_64.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-afm-2007-57.el6_2.x86_64.rpm
  test -f texlive-texmf-errata-xetex-2007-7.1.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-errata-xetex-2007-7.1.el6.noarch.rpm
  test -f texlive-texmf-errata-doc-2007-7.1.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-errata-doc-2007-7.1.el6.noarch.rpm
  test -f texlive-texmf-errata-fonts-2007-7.1.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-errata-fonts-2007-7.1.el6.noarch.rpm
  test -f texlive-texmf-doc-2007-38.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-doc-2007-38.el6.noarch.rpm
  test -f texlive-texmf-dvips-2007-38.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-dvips-2007-38.el6.noarch.rpm
  test -f texlive-utils-2007-57.el6_2.x86_64.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-utils-2007-57.el6_2.x86_64.rpm
  test -f texlive-texmf-latex-2007-38.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-latex-2007-38.el6.noarch.rpm
  test -f texlive-texmf-context-2007-38.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-context-2007-38.el6.noarch.rpm
  test -f texlive-context-2007-57.el6_2.x86_64.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-context-2007-57.el6_2.x86_64.rpm
  test -f texlive-xetex-2007-57.el6_2.x86_64.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-xetex-2007-57.el6_2.x86_64.rpm
  test -f texlive-texmf-errata-dvips-2007-7.1.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-errata-dvips-2007-7.1.el6.noarch.rpm
  test -f texlive-texmf-errata-latex-2007-7.1.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-errata-latex-2007-7.1.el6.noarch.rpm
  test -f texlive-texmf-xetex-2007-38.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-xetex-2007-38.el6.noarch.rpm
  test -f texlive-dviutils-2007-57.el6_2.x86_64.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-dviutils-2007-57.el6_2.x86_64.rpm
  test -f texlive-texmf-errata-2007-7.1.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-errata-2007-7.1.el6.noarch.rpm
  test -f texlive-texmf-afm-2007-38.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-afm-2007-38.el6.noarch.rpm
  test -f texlive-texmf-errata-afm-2007-7.1.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-errata-afm-2007-7.1.el6.noarch.rpm
  test -f texlive-texmf-errata-context-2007-7.1.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/centos/6.6/os/x86_64/Packages/texlive-texmf-errata-context-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh texlive-texmf-2007-38.el6.noarch.rpm
  sudo rpm -ivh texlive-texmf-errata-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh texlive-texmf-errata-fonts-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh texlive-texmf-fonts-2007-38.el6.noarch.rpm
  sudo rpm -ivh texlive-texmf-errata-afm-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh texlive-texmf-afm-2007-38.el6.noarch.rpm
  sudo rpm -ivh texlive-afm-2007-57.el6_2.x86_64.rpm
  sudo rpm -ivh texlive-texmf-errata-xetex-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh texlive-texmf-errata-doc-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh texlive-texmf-doc-2007-38.el6.noarch.rpm
  sudo rpm -ivh texlive-texmf-dvips-2007-38.el6.noarch.rpm
  sudo rpm -ivh texlive-utils-2007-57.el6_2.x86_64.rpm
  sudo rpm -ivh texlive-texmf-latex-2007-38.el6.noarch.rpm
  sudo rpm -ivh texlive-texmf-errata-context-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh texlive-texmf-context-2007-38.el6.noarch.rpm
  sudo rpm -ivh texlive-context-2007-57.el6_2.x86_64.rpm
  sudo rpm -ivh texlive-xetex-2007-57.el6_2.x86_64.rpm
  sudo rpm -ivh texlive-texmf-errata-dvips-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh texlive-texmf-errata-latex-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh texlive-texmf-xetex-2007-38.el6.noarch.rpm
  sudo rpm -ivh texlive-dviutils-2007-57.el6_2.x86_64.rpm
  sudo rpm -ivh texlive-texmf-errata-2007-7.1.el6.noarch.rpm
  sudo rpm -ivh texlive-2007-57.el6_2.x86_64.rpm

  test -f asciidoc-8.6.9.tar.gz || wget http://sourceforge.net/projects/asciidoc/files/asciidoc/8.6.9/asciidoc-8.6.9.tar.gz/download -O asciidoc-8.6.9.tar.gz
  if [ "$INSTALLING_FROM_VIRTUALBOX_SHARED_FOLDER" == "true" ]; then
    # for whatever reason you can't install this from a virtualbox shared folder
    rm -rf $HOOT_CORE_INSTALL_DIR/asciidoc-8.6.9
    cp asciidoc-8.6.9.tar.gz $HOOT_CORE_INSTALL_DIR
    cd $HOOT_CORE_INSTALL_DIR
    tar -xzf asciidoc-8.6.9.tar.gz
  else
    rm -rf asciidoc-8.6.9 
  fi
  tar -xzf asciidoc-8.6.9.tar.gz
  cd asciidoc-8.6.9
  autoconf && ./configure --prefix=$HOOT_CORE_INSTALL_DIR && make -j8 && sudo make install
  cd ..

  rm -rf dblatex-0.2.8
  test -f dblatex-0.2.8.tar.bz2 || wget http://downloads.sourceforge.net/project/dblatex/dblatex/dblatex-0.2.8/dblatex-0.2.8.tar.bz2
  tar -xjf dblatex-0.2.8.tar.bz2
  cd dblatex-0.2.8
  sudo python setup.py install --prefix=$HOOT_CORE_INSTALL_DIR
  cd ..

  test -f python-numpy-1.6.1-17.1.x86_64.rpm || wget ftp://ftp.pbone.net/mirror/ftp5.gwdg.de/pub/opensuse/repositories/home:/strohel/CentOS_CentOS-6/x86_64/python-numpy-1.6.1-17.1.x86_64.rpm
  test -f python-numpy-devel-1.6.1-17.1.x86_64.rpm || wget ftp://ftp.pbone.net/mirror/ftp5.gwdg.de/pub/opensuse/repositories/home:/strohel/CentOS_CentOS-6/x86_64/python-numpy-devel-1.6.1-17.1.x86_64.rpm
  sudo rpm -ivh python-numpy-1.6.1-17.1.x86_64.rpm 
  sudo rpm -ivh python-numpy-devel-1.6.1-17.1.x86_64.rpm

  #TODO: this section doesn't work yet
  export PYTHONPATH=$HOOT_CORE_INSTALL_DIR/lib64/python-2.6/site-packages
  rm -rf matplotlib-1.3.1
  test -f matplotlib-1.3.1.tar.gz || wget http://sourceforge.net/projects/matplotlib/files/matplotlib/matplotlib-1.3.1/matplotlib-1.3.1.tar.gz/download -O matplotlib-1.3.1.tar.gz
  tar -xzf matplotlib-1.3.1.tar.gz
  cd matplotlib-1.3.1
  python ./setup.py clean && python ./setup.py build && sudo python ./setup.py install --prefix=$HOOT_CORE_INSTALL_DIR
  cd ..

  echo "Installing database packages and configuring database..."

  test -f postgresql91-libs-9.1.11-1PGDG.rhel6.x86_64.rpm || wget http://mirror.its.sfu.ca/mirror/CentOS-Third-Party/pgrpm/pgrpm-91/redhat/rhel-6.5-x86_64/postgresql91-libs-9.1.11-1PGDG.rhel6.x86_64.rpm
  test -f postgresql91-9.1.11-1PGDG.rhel6.x86_64.rpm || wget http://mirror.its.sfu.ca/mirror/CentOS-Third-Party/pgrpm/pgrpm-91/redhat/rhel-6.5-x86_64/postgresql91-9.1.11-1PGDG.rhel6.x86_64.rpm
  test -f postgresql91-server-9.1.11-1PGDG.rhel6.x86_64.rpm || wget http://mirror.its.sfu.ca/mirror/CentOS-Third-Party/pgrpm/pgrpm-91/redhat/rhel-6.5-x86_64/postgresql91-server-9.1.11-1PGDG.rhel6.x86_64.rpm
  test -f postgresql91-contrib-9.1.11-1PGDG.rhel6.x86_64.rpm || wget http://mirror.its.sfu.ca/mirror/CentOS-Third-Party/pgrpm/pgrpm-91/redhat/rhel-6.5-x86_64/postgresql91-contrib-9.1.11-1PGDG.rhel6.x86_64.rpm
  test -f postgresql91-devel-9.1.11-1PGDG.rhel6.x86_64.rpm || wget http://mirror.its.sfu.ca/mirror/CentOS-Third-Party/pgrpm/pgrpm-91/redhat/rhel-6.5-x86_64/postgresql91-devel-9.1.11-1PGDG.rhel6.x86_64.rpm
  sudo rpm -ivh postgresql91-libs-9.1.11-1PGDG.rhel6.x86_64.rpm
  sudo rpm -ivh postgresql91-9.1.11-1PGDG.rhel6.x86_64.rpm
  sudo rpm -ivh postgresql91-server-9.1.11-1PGDG.rhel6.x86_64.rpm
  sudo rpm -ivh postgresql91-contrib-9.1.11-1PGDG.rhel6.x86_64.rpm
  sudo rpm -ivh postgresql91-devel-9.1.11-1PGDG.rhel6.x86_64.rpm
  sudo service postgresql-9.1 initdb
  sudo service postgresql-9.1 start
  # set Postgres to autostart
  sudo /sbin/chkconfig --add postgresql-9.1
  sudo /sbin/chkconfig postgresql-9.1 on
  test -f postgis2_91-2.0.4-1.rhel6.x86_64.rpm || wget http://mirror.its.sfu.ca/mirror/CentOS-Third-Party/pgrpm/pgrpm-91/redhat/rhel-6.5-x86_64/postgis2_91-2.0.4-1.rhel6.x86_64.rpm
  sudo rpm -ivh postgis2_91-2.0.4-1.rhel6.x86_64.rpm
  #test -f liquibase-3.1.0-1.el6.noarch.rpm || wget ftp://ftp.univie.ac.at/systems/linux/fedora/epel/6/i386/liquibase-3.1.0-1.el6.noarch.rpm
  test -f liquibase-3.1.0-1.el6.noarch.rpm || wget ftp://ftp.muug.mb.ca/mirror/fedora/epel/6/x86_64/liquibase-3.1.0-1.el6.noarch.rpm
  sudo rpm -ivh liquibase-3.1.0-1.el6.noarch.rpm
  if ! sudo -u postgres psql -lqt | grep -i --quiet hoot; then
    echo "Creating Services Database"
    sudo -u postgres createuser --superuser hoot
    sudo -u postgres psql -c "alter user hoot with password 'hoottest';"
    sudo -u postgres createdb hoot --owner=hoot
    sudo -u postgres createdb wfsstoredb --owner=hoot
    sudo -u postgres psql -d hoot -c 'create extension hstore;'
    sudo -u postgres psql -d postgres -c "UPDATE pg_database SET datistemplate='true' WHERE datname='wfsstoredb'"
    sudo -u postgres psql -d wfsstoredb -f /usr/pgsql-9.1/share/contrib/postgis-2.0/postgis.sql
    sudo -u postgres psql -d wfsstoredb -f /usr/pgsql-9.1/share/contrib/postgis-2.0/spatial_ref_sys.sql
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on geometry_columns TO PUBLIC;"
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on geography_columns TO PUBLIC;"
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on spatial_ref_sys TO PUBLIC;"
  fi
  if ! grep -i --quiet HOOT /var/lib/pgsql/9.1/data/postgresql.conf; then
    echo "Tuning PostgreSQL"
    sudo -u postgres sed -i.bak s/^max_connections/\#max_connections/ /var/lib/pgsql/9.1/data/postgresql.conf
    sudo -u postgres sed -i.bak s/^shared_buffers/\#shared_buffers/ /var/lib/pgsql/9.1/data/postgresql.conf
    sudo -u postgres bash -c "cat >> /var/lib/pgsql/9.1/data/postgresql.conf" <<EOT
#--------------
# Hoot Settings
#--------------
max_connections = 1000
shared_buffers = 1024MB
max_files_per_process = 1000
work_mem = 16MB
maintenance_work_mem = 256MB
checkpoint_segments = 20
autovacuum = off
EOT
  fi
  if ! sysctl -e kernel.shmmax | grep --quiet 1173741824; then
    echo "Setting kernel.shmmax"
    sudo sysctl -w kernel.shmmax=1173741824
    sudo sh -c "echo 'kernel.shmmax=1173741824' >> /etc/sysctl.conf"
  fi
  if ! sysctl -e kernel.shmall | grep --quiet 2097152; then
    echo "Setting kernel.shmall"
    sudo sysctl -w kernel.shmall=2097152
    sudo sh -c "echo 'kernel.shmall=2097152' >> /etc/sysctl.conf"
  fi
  if ! grep --quiet PSQL_HOME /etc/profile; then
    echo 'export PSQL_HOME=/usr/pgsql-9.1' >> /etc/profile
    source /etc/profile
  fi
  sudo service postgresql-9.1 restart

  echo "Installing GDAL..."
  test -f FileGDB_API_1_3-64.tar.gz || wget http://downloads2.esri.com/Software/FileGDB_API_1_3-64.tar.gz
  sudo rm -rf $HOOT_CORE_INSTALL_DIR/FileGDB_API-1.3
  sudo tar -xzf FileGDB_API_1_3-64.tar.gz --directory /usr/local
  sudo cp -R FileGDB_API $HOOT_CORE_INSTALL_DIR/FileGDB_API-1.3
  sudo cp $HOOT_CORE_INSTALL_DIR/FileGDB_API-1.3/lib/* $HOOT_CORE_INSTALL_DIR/lib/
  rm -rf gdal-1.10.1
  test -f gdal-1.10.1.tar.gz || wget http://download.osgeo.org/gdal/1.10.1/gdal-1.10.1.tar.gz
  tar -xzf gdal-1.10.1.tar.gz
  cd gdal-1.10.1
  ./configure -q --with-pg=/usr/pgsql-9.1/bin/pg_config --with-fgdb=$HOOT_CORE_INSTALL_DIR/FileGDB_API/ --with-python --prefix=$HOOT_CORE_INSTALL_DIR && make -j8 && sudo make install
  cd swig/python
  python setup.py build && sudo python setup.py install
  sudo ldconfig
  cd ../../..
  
  echo "Installing Qt..."
  sudo yum -y remove qt*
  sudo rm -f $HOOT_CORE_INSTALL_DIR/qt-everywhere-opensource-src-4.8.6.tar.gz
  rm -rf qt-everywhere-opensource-src-4.8.6
  sudo rm -rf $HOOT_CORE_INSTALL_DIR/qt-everywhere-opensource-src-4.8.6
  test -f qt-everywhere-opensource-src-4.8.6.tar.gz || wget http://download.qt-project.org/official_releases/qt/4.8/4.8.6/qt-everywhere-opensource-src-4.8.6.tar.gz
  sudo cp qt-everywhere-opensource-src-4.8.6.tar.gz $HOOT_CORE_INSTALL_DIR
  cd $HOOT_CORE_INSTALL_DIR
  sudo tar -xzf qt-everywhere-opensource-src-4.8.6.tar.gz
  cd qt-everywhere-opensource-src-4.8.6
  ./configure -opensource -prefix /usr/lib64/qt4 -no-opengl -no-xvideo -no-xinput -no-xkb -no-cups -no-webkit -no-qt3support -fast -plugin-sql-psql -I/usr/pgsql-9.1/include -L/usr/pgsql-9.1/lib -nomake demos -nomake examples -confirm-license -silent --prefix=$HOOT_CORE_INSTALL_DIR && make -j8 && sudo make install
  cd $INSTALL_SOURCE_DIR
  sudo rm -f /usr/bin/qmake
  sudo ln -s /usr/lib64/qt4/qmake /usr/bin/qmake
  if ! grep --quiet QT_HOME /etc/profile; then
    #TODO: replace /usr/local with $HOOT_CORE_INSTALL_DIR
    echo 'export QT_HOME=/usr/local/qt-everywhere-opensource-src-4.8.6' >> /etc/profile
    source /etc/profile
  fi

  if ! grep --quiet NODE_PATH /etc/profile; then
    echo "Installing node js dependencies"
    sudo npm config set registry http://registry.npmjs.org/
    sudo npm install -g xml2js htmlparser imagemagick mocha@1.20.1 express@3.1.2 async html-to-text restler
    echo 'Adding NODE_PATH to user environment'
    echo 'export NODE_PATH=/usr/local/lib/node_modules' >> /etc/profile
    source /etc/profile
  fi

  echo "Configuring Tomcat..."
  sudo service tomcat6 start
  #TODO: use $HOOT_CORE_INSTALL_DIR here instead of /usr/local
  if ! grep -i --quiet HOOT /usr/sbin/tomcat6; then
    echo "Configuring tomcat6 environment"
    sudo bash -c "cat >> /usr/sbin/tomcat6" <<EOT
#--------------
# Hoot Settings
#--------------
HOOT_HOME=/usr/local/hoot
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:$HOOT_HOME/lib:$HOOT_HOME/pretty-pipes/lib
GDAL_DATA=/usr/local/share/gdal
GDAL_LIB_DIR=/usr/local/lib
HOOT_WORKING_NAME=hoot
QT_HOME=/usr/local/qt-everywhere-opensource-src-4.8.6
PSQL_HOME=/usr/pgsql-9.1
PATH=$HOOT_HOME/bin:/usr/local/lib:/usr/local/bin:$PATH:$QT_HOME/bin:$PSQL_HOME/bin
EOT
  fi
  # Create Tomcat context path for tile images
  if ! grep -i --quiet 'ingest/processed' /etc/tomcat6/server.xml; then
    echo "Adding Tomcat context path for tile images"
    sudo sed -i.bak "s@<\/Host>@ <Context docBase=\"\/usr\/local\/hoot\/ingest\/processed\" path=\"\/static\" \/>\n &@" /etc/tomcat6/server.xml
  fi
  # Allow linking in Tomcat context
  if ! grep -i --quiet 'allowLinking="true"' /etc/tomcat6/context.xml; then
    echo "Set allowLinking to true in Tomcat context"
    sudo sed -i.bak "s@^<Context>@<Context allowLinking=\"true\">@" /etc/tomcat6/context.xml
  fi
  # Set Tomcat to autostart
  sudo /sbin/chkconfig --add tomcat6
  sudo /sbin/chkconfig tomcat6 on
  sudo service tomcat6 restart 

  echo "Configuring firewall..."
  sudo iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 80 -j ACCEPT
  sudo iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 8080 -j ACCEPT
  sudo iptables -A PREROUTING -t nat -p tcp --dport 80 -j REDIRECT --to-ports 8080
  sudo iptables -A OUTPUT -t nat -s 0/0 -d 127/8 -p tcp --dport 80 -j REDIRECT --to-ports 8080
  sudo service iptables save
  sudo service iptables restart
  sudo service tomcat6 restart 

  echo "Configuring path..."
  JAVA_HOME=/usr/lib/jvm/`ls /usr/lib/jvm | grep -e java-1.7.*-openjdk-*.x86_64`
  if ! grep --quiet 'JAVA_HOME' /etc/profile; then
    echo 'export JAVA_HOME='$JAVA_HOME >> /etc/profile
    source /etc/profile
  fi
  if ! grep --quiet 'export PATH=' /etc/profile; then
    echo 'export PATH=/usr/local/lib:/usr/local/bin:$PATH:$QT_HOME/bin:$PSQL_HOME/bin:' >> /etc/profile
    source /etc/profile
  fi

fi

if [ "$INSTALL_HOOT_APPLICATION" == "true" ]; then

  if [ "$JAVA_HOME" == "" ]; then
    if [ "$UPDATE_SERVICES" == "true" ]; then 
      echo "Please populate JAVA_HOME."
      echo "Its usually located at something like: /usr/lib/jvm/java-1.7.0-openjdk-*.x86_64"
      exit -1
    fi
  fi
  if [ "$HOOT_VERSION" == "" ]; then
    echo "Please enter a HOOT_VERSION.  See script comments for details."
    exit -1
  fi
  if [ "$CORE_INSTALL_USER_GROUP" == "" ]; then
    echo "Please enter a CORE_INSTALL_USER_GROUP.  See script comments for details."
    exit -1
  fi
  if [ "$CORE_RUNTIME_USER" == "" ]; then
    echo "Please enter a CORE_RUNTIME_USER.  See script comments for details."
    exit -1
  fi
  if [ "$CORE_RUNTIME_USER_GROUP" == "" ]; then
    echo "Please enter a CORE_RUNTIME_USER_GROUP.  See script comments for details."
    exit -1
  fi
  if [ "$HOOT_CORE_INSTALL_DIR" == "" ]; then
    echo "Please enter a HOOT_CORE_INSTALL_DIR.  See script comments for details."
    exit -1
  fi

  cd $TEMP_DIR
  mkdir -p $TEMP_DIR/$BACKUP_DIR_NAME

  cd $INSTALL_SOURCE_DIR

  # grab hoot
  test -f hootenanny-$HOOT_VERSION.tar.gz || wget https://github.com/ngageoint/hootenanny/releases/download/v$HOOT_VERSION/hootenanny-$HOOT_VERSION.tar.gz
  test -f hootenanny-services-$HOOT_VERSION.war || wget https://github.com/ngageoint/hootenanny/releases/download/v$HOOT_VERSION/hootenanny-services-$HOOT_VERSION.war
  tar -xzf hootenanny-$HOOT_VERSION.tar.gz

  if [ "$UPDATE_DATABASE" == "true" ]; then
    if [ "$CLEAR_DATABASE" == "true" ]; then
      echo "Clear the database?  Are you ABSOLUTELY SURE? y/n?"
      read clear
      if [ "$clear" == "y" ]; then
        echo "Clearing the hoot database..."
        export HOOT_HOME=hootenanny-$HOOT_VERSION
        if [-e hootenanny-$HOOT_VERSION/conf/DatabaseConfigLocal.sh ]; then
          rm hootenanny-$HOOT_VERSION/conf/DatabaseConfigLocal.sh
        fi
        hootenanny-$HOOT_VERSION/scripts/DeleteAllTables.sh
        export HOOT_HOME=
        echo "Database cleared."
      else
        echo "*NOT* clearing the hoot database..."
      fi
    fi
    echo
    echo "Updating the hoot database..."
    # This will apply the latest database schema update to the hoot database.  If the schema has not
    # changed since the last install, no change occurs.
    rm -rf hootenanny-services-temp
    mkdir hootenanny-services-temp
    cp hootenanny-services-$HOOT_VERSION.war hootenanny-services-temp
    cd hootenanny-services-temp
    unzip -q hootenanny-services-$HOOT_VERSION.war
    cd ..
    mv hootenanny-services-temp hootenanny-services-$HOOT_VERSION
    # liquibase bases its decision on whether to apply an update partially on what
    # directory each changeset XML file resides in.  Therefore, you must execute
    # this command exactly as written, from the exact directory shown here.  If not,
    # unnecessary updates will be applied, which will result in SQL errors.
    cd hootenanny-services-$HOOT_VERSION/WEB-INF
    liquibase --contexts=default,production --changeLogFile=classes/db/db.changelog-master.xml --promptForNonLocalDatabase=false --defaultsFile=classes/db/liquibase.properties --logLevel=warning --classpath=lib/postgresql-9.4.1208.jre7.jar --url jdbc:postgresql:hoot update #&> /usr/local/test
    cd $TEMP_DIR
    rm -rf hootenanny-services-$HOOT_VERSION
    echo "hoot database updated."
    echo
    sleep 3
  fi

  if [ "$UPDATE_CORE" == "true" ]; then

    # Hootenanny core is the command line application, which executes the conflation algorithms.
    echo "Updating the hoot core..."

    if [ -d "$HOOT_CORE_INSTALL_DIR/customscript" ]; then
      # back up the custom translations; assumes install dir for this installation was the same as
      # the previous installation
      sudo cp -R $HOOT_CORE_INSTALL_DIR/customscript $TEMP_DIR
    fi

    if [ "$REMOVE_EXISTING_CORE_INSTALL" == "true" ]; then
	    # This directory could be left over from a previous failed install.
	    if [ -d "$HOOT_CORE_INSTALL_DIR/hootenanny-$HOOT_VERSION" ]; then
    	  sudo rm -rf $HOOT_CORE_INSTALL_DIR/hootenanny-$HOOT_VERSION
	    fi
	    # No need to backup the hoot core dir, since we leave old versions copied and just update the 
      # symbolic link for new versions.
      sudo cp -R hootenanny-$HOOT_VERSION $HOOT_CORE_INSTALL_DIR
      if [ -d "$HOOT_CORE_INSTALL_DIR/hoot" ]; then
        cd $HOOT_CORE_INSTALL_DIR/hoot
        sudo make uninstall
        cd ..
        sudo rm -f $HOOT_CORE_INSTALL_DIR/hoot 
        sudo rm -f $HOOT_CORE_INSTALL_DIR/bin/hoot
      fi
    fi

    sudo chown -R $USER:$CORE_INSTALL_USER_GROUP hootenanny-$HOOT_VERSION
    cd hootenanny-$HOOT_VERSION
    # building the docs requires git to generate dates, which we don't have in the production 
    # environment...so just backing up the docs already included in the release and redeploying them 
    # to the target install dir instead
    sudo cp -R docs $TEMP_DIR
    source ./SetupEnv.sh
    ./configure -q --prefix=$HOOT_CORE_INSTALL_DIR/hootenanny-$HOOT_VERSION $CORE_CONFIGURATION_OPTIONS
    if [ "$REMOVE_EXISTING_CORE_INSTALL" == "true" ]; then
      # This is here in case a previous install was stopped short and you want a clean build.
      make -s clean-all
    fi
    make -sj8 && sudo make -s install
    sudo cp -R $TEMP_DIR/docs .

    if [ -d "$HOOT_CORE_INSTALL_DIR/customscript" ]; then
      # restore the custom translations
      sudo cp -R $TEMP_DIR/customscript .
    fi

    # configure asciidoc and matplotlib
    sudo mkdir -p $ASCIIDOC_FILTERS_HOME/mpl
    sudo mkdir -p $ASCIIDOC_FILTERS_HOME/gnuplot
    sudo cp -rf $HOOT_CORE_INSTALL_DIR/hoot/docs/filters/mpl $ASCIIDOC_FILTERS_HOME
    sudo cp -rf $HOOT_CORE_INSTALL_DIR/hoot/docs/filters/gnuplot $ASCIIDOC_FILTERS_HOME

    # configure hoot core permissions
    cd ..
    sudo ln -s $HOOT_CORE_INSTALL_DIR/hootenanny-$HOOT_VERSION $HOOT_CORE_INSTALL_DIR/hoot
    sudo chmod -R 755 $HOOT_CORE_INSTALL_DIR/hoot
    sudo chown -R $CORE_RUNTIME_USER:$CORE_RUNTIME_USER_GROUP $HOOT_CORE_INSTALL_DIR/hoot
    sudo chmod -R 755 hootenanny-$HOOT_VERSION
    sudo chown -R $CORE_RUNTIME_USER:$CORE_RUNTIME_USER_GROUP hootenanny-$HOOT_VERSION
    sudo ln -s $HOOT_CORE_INSTALL_DIR/hoot/bin/hoot $HOOT_CORE_INSTALL_DIR/bin/hoot
    hoot --version

    echo "Hoot core updated."
    echo
    sleep 3

  fi
  
  if [ "$UPDATE_FOUO_TRANSLATIONS" == "true" ]; then

    echo "Updating the FOUO translations..."

    cd $TEMP_DIR
    # remove any existing translation refs
    rm -rf plugins-local
    rm -rf translations-local

    tar -xzf hootenanny-translations-$HOOT_VERSION.tar.gz > /dev/null
	  # No need to backup these translations, since they are kept under the hoot root directory
	  # with previous versions.
    sudo cp -R plugins-local $HOOT_CORE_INSTALL_DIR/hoot
	  rm -rf plugins-local

    # set translation permissions
    sudo chown -R $CORE_RUNTIME_USER:$CORE_RUNTIME_USER_GROUP $HOOT_CORE_INSTALL_DIR/hoot/plugins-local
    sudo chmod -R 755 $HOOT_CORE_INSTALL_DIR/hoot/plugins-local
    sudo cp -R translations-local $HOOT_CORE_INSTALL_DIR/hoot
	  rm -rf translations-local
    sudo chown -R $CORE_RUNTIME_USER:$CORE_RUNTIME_USER_GROUP $HOOT_CORE_INSTALL_DIR/hoot/translations-local
    sudo chmod -R 755 $HOOT_CORE_INSTALL_DIR/hoot/translations-local

    echo "FOUO translations updated."
    echo
    sleep 3

  fi

  if [ "$UPDATE_SERVICES" == "true" ]; then

    echo "Updating the hoot services..."

    cd $TOMCAT_HOME

    # deegree config
	  sudo mkdir -p $DEEGREE_DIR_ROOT/.deegree
	  sudo chown root:$CORE_RUNTIME_USER_GROUP $DEEGREE_DIR_ROOT/.deegree	

    # backup any existing basemaps
    if [ -d "$TEMP_DIR/BASEMAPS" ]; then
      rm -rf $TEMP_DIR/BASEMAPS
    fi
    if [ -d "$BASEMAP_HOME/BASEMAPS" ]; then
      sudo cp -R $BASEMAP_HOME/BASEMAPS $TEMP_DIR
    fi

    # backup the web services
	  if [ "$BACKUP_DIR_NAME" != "" ]; then
      if [ -f "hoot-services.war" ]; then
  	    sudo mv hoot-services.war $TEMP_DIR/$BACKUP_DIR_NAME/hootenanny-services-$DATE.war
      fi
	  else
      if [ -f "hoot-services.war" ]; then
		    rm -f hoot-services.war
      fi
	  fi
    # just in case any other war files are in there...
    sudo rm -f hoot*-services*.war

    # backup the customized local conf
    if [ -f hoot-services/WEB-INF/classes/conf/local.conf ]; then
      sudo cp hoot-services/WEB-INF/classes/conf/local.conf $TEMP_DIR
      SERVICES_LOCAL_CONF_FILE_EXISTS=true
      echo "Found existing services local conf file.  Backed up to $TEMP_DIR."
    else
      SERVICES_LOCAL_CONF_FILE_EXISTS=false
      echo "No existing services local.conf file found.  No backup of it needs to be made."
    fi

    if [ -d "hoot-services" ]; then
      sudo rm -rf hoot-services
    fi

    # install the web services
    sudo chmod 775 $TEMP_DIR/hootenanny-services-$HOOT_VERSION.war
    sudo chown root:$CORE_RUNTIME_USER_GROUP $TEMP_DIR/hootenanny-services-$HOOT_VERSION.war
    sudo cp $TEMP_DIR/hootenanny-services-$HOOT_VERSION.war hoot-services.war
    echo "Waiting for web server to extract war file initially..."
    sleep 10

    # restore the local conf
    if [ "$SERVICES_LOCAL_CONF_FILE_EXISTS" == "true" ]; then
      sudo cp $TEMP_DIR/local.conf webapps/hoot-services/WEB-INF/classes/conf
      echo "Copied backed up version of services local.conf file."
    fi

    # restore basemaps
    if [ -d "$BASEMAP_HOME/BASEMAPS" ]; then
  	  sudo rm -rf $BASEMAP_HOME/BASEMAPS
	  fi
    if [ -d "$TEMP_DIR/BASEMAPS" ]; then
      sudo cp -R $TEMP_DIR/BASEMAPS $BASEMAP_HOME
    fi
  
    # set web services permissions
    sudo chown -R root:$CORE_RUNTIME_USER_GROUP hoot-services
    sudo chmod -R 775 hoot-services
	  # needed by WFS
    sudo chmod -R 777 hoot-services/WEB-INF/workspace

    sudo service tomcat6 restart
    echo "Waiting for web server to restart..."
    sleep 10

    echo "hoot services updated."
    echo
    sleep 3

  fi

  if [ "$UPDATE_UI" == "true" ]; then

    echo "Updating the hoot ui..."

	  cd $TOMCAT_HOME

    # back up any custom basemap refs
    if [ -f hootenanny-id/data/imagery.json ]; then
      cp hootenanny-id/data/imagery.json $TEMP_DIR
      IMAGERY_FILE_EXISTS=true
      echo "Found existing imagery.json file and backed up to: " $TEMP_DIR
    else
      IMAGERY_FILE_EXISTS=false
      echo "No existing imagery.json file found.  Using default."
    fi

    # Install the web app
    # No need to first back up hoot iD, since the previous version will exist in the last hoot 
    # core install dir.
    sudo rm -rf hootenanny-id
    sudo cp -R $HOOT_CORE_INSTALL_DIR/hoot/hoot-ui/ hootenanny-id
    sudo chown -R root:$CORE_RUNTIME_USER_GROUP hootenanny-id
    sudo chmod -R +r hootenanny-id
    sudo find hootenanny-id -type d -exec chmod +x {} ';' # TODO: What does this do?

    # restore basemaps
    if [ "$IMAGERY_FILE_EXISTS" == "true" ]; then
      echo "Restoring backed up custom basemaps..."
      cd hootenanny-id/data
      sudo cp $TEMP_DIR/imagery.json hootenanny-id/data
    fi

    echo "hoot ui updated."
    echo
    sleep 3

  fi

  echo "Hootenanny update complete to version $HOOT_VERSION.  Access the command line interface by typing: hoot."
  if [ "$UPDATE_UI" == "true" ]; then
    echo "View the web interface at " $BASE_SERVICES_URL"/hootenanny-id"
    echo "Verify the correct version numbers by clicking on the InnoVision logo."
  else
    echo "Verify the correct version is installed with by typing: hoot --version."
  fi
  #cd $TEMP_DIR

fi
