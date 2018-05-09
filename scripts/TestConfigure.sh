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
    HootTest --quick --parallel $(nproc)
    make -s clean
}

# Try a lot of the configure combinations to see if there is a broken .pro file.

echo "### 01. --without-cppunit"
./configure -q --without-cppunit                                                                    && build_notest

echo "### 02. --without-newmat"
./configure -q                   --without-newmat                                                   && build

echo "### 03. --without-log4cxx"
./configure -q                                    --without-log4cxx                                 && build

echo "### 04. --with-services"
./configure -q                                                      --with-services                 && build

echo "### 05. --without-cppunit --without-newmat"
./configure -q --without-cppunit --without-newmat                                                   && build_notest

echo "### 06. --without-cppunit --without-newmat --without-log4cxx"
./configure -q --without-cppunit --without-newmat --without-log4cxx                                 && build_notest

echo "### 07. --without-cppunit --without-newmat --without-log4cxx --with-services"
./configure -q --without-cppunit --without-newmat --without-log4cxx --with-services                 && build_notest

echo "### 08. --without-newmat --with-services"
./configure -q                   --without-newmat                   --with-services                 && build

echo "### 09. --without-newmat --without-log4cxx"
./configure -q                   --without-newmat --without-log4cxx                                 && build

echo "### 10. --without-newmat --without-log4cxx --with-services"
./configure -q                   --without-newmat --without-log4cxx --with-services                 && build

echo "### 11. --without-log4cxx --with-services"
./configure -q                                    --without-log4cxx --with-services                 && build

echo "### 12. ./configure -q"
./configure -q

echo "### 13. --without-cppunit --without-newmat --without-log4cxx --with-services --with-rnd"
./configure -q --without-cppunit --without-newmat --without-log4cxx --with-services --with-rnd      && build_notest

echo "### 14. --without-newmat --without-log4cxx --with-services --with-rnd"
./configure -q                   --without-newmat --without-log4cxx --with-services --with-rnd      && build

echo "### 15. --with-rnd"
./configure -q                                                                      --with-rnd      && build

echo "############"
echo "# Success! #"
echo "############"
exit 0
