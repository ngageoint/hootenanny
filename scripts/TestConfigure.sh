#!/bin/bash

set -e

aclocal && autoconf && autoheader && automake --add-missing && ./configure -q && make -s clean

function build_notest {
    make -s clean
    make -sj`nproc`
    hoot version
    make -s clean
}

function build {
    make -s clean
    make -sj`nproc`
    HootTest --quick --parallel `nproc`
    make -s clean
}

# Try a lot of the configure combinations to see if there is a broken .pro file.

echo "#######################################################################"
echo "### 01. --without-cppunit                                             #"
echo "#######################################################################"
./configure -q --without-cppunit                                                                    && build_notest

echo "#######################################################################"
echo "### 02. --without-newmat                                              #"
echo "#######################################################################"
./configure -q                   --without-newmat                                                   && build

echo "#######################################################################"
echo "### 03. --with-services                                               #"
echo "#######################################################################"
./configure -q                                    --with-services                                   && build

echo "#######################################################################"
echo "### 04. --without-cppunit --without-newmat                            #"
echo "#######################################################################"
./configure -q --without-cppunit --without-newmat                                                   && build_notest

echo "#######################################################################"
echo "### 05. --without-cppunit --without-newmat --with-services            #"
echo "#######################################################################"
./configure -q --without-cppunit --without-newmat --with-services                                   && build_notest

echo "#######################################################################"
echo "### 06. --without-newmat --with-services                              #"
echo "#######################################################################"
./configure -q                   --without-newmat --with-services                                   && build

echo "#######################################################################"
echo "### 07. ./configure -q                                                #"
echo "#######################################################################"
./configure -q

echo "#######################################################################"
echo "### 08. --without-cppunit --without-newmat --with-services --with-rnd #"
echo "#######################################################################"
./configure -q --without-cppunit --without-newmat --with-services                   --with-rnd      && build_notest

echo "#######################################################################"
echo "### 09. --without-newmat --with-services --with-rnd                   #"
echo "#######################################################################"
./configure -q                   --without-newmat --with-services                   --with-rnd      && build

echo "#######################################################################"
echo "### 10. --with-rnd                                                    #"
echo "#######################################################################"
./configure -q                                                                      --with-rnd      && build

echo "#######################################################################"
echo "# Success!                                                            #"
echo "#######################################################################"
exit 0
