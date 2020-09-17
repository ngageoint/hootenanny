#!/usr/bin/env bash

set -e

#####
# Cleanup stuff that we don't need in the final image
# This aims to make the final docker image smaller
#
# MattJ

echo "Removeing Packages"

yum remove -q -y \
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

echo "Autoremove Packages"
yum -y autoremove

# Now put some stuff back that got whacked by autoremove
echo "Re-install some packages"
yum install -q -y \
  boost-iostreams \
  boost-system \
  cppunit \
  qt-x11 \
  v8

echo "Yum clean and remove test files"
yum clean all
rm -rf /var/cache/yum

# We don't need the test files anymore
rm -rf /var/lib/hootenanny/test-files/*
rm -rf /var/lib/hootenanny/test-output
##########################################
