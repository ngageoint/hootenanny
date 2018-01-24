#!/usr/bin/env bash

set -e

#####
# Cleanup stuff that we don't need in the final image
# This aims to make the final docker image smaller
#
# MattJ

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

yum -y autoremove

# Now put some stuff back that got whacked by autoremove
yum install -q -y \
  boost-iostreams \
  boost-system \
  cppunit \
  qt-x11 \
  v8

yum clean all
rm -rf /var/cache/yum
##########################################
