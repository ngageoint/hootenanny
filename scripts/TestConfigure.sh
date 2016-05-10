#!/bin/bash

set -e

aclocal && autoconf && autoheader && automake && ./configure -q && make -s clean

function build_notest {
    make -s clean
    make -sj`nproc`
    hoot --version
    # make sure hadoop is enabled
    hoot --help | grep -- big-conflate &> /dev/null
    make -s clean
}

function build {
    make -s clean
    make -sj`nproc`
    HootTest --quick
    # make sure hadoop is enabled
    hoot --help | grep -- big-conflate &> /dev/null
    make -s clean
}

# Try a lot of the configure combinations to see if there is a broken .pro file.
# Trying all of the combinations would take 32 runs.
echo "### 01. --without-cppunit"
./configure -q --without-cppunit                                                                    && build_notest

echo "### 02. --without-stxxl"
./configure -q                                                                      --without-stxxl && build

echo "### 03. --without-newmat"
./configure -q                   --without-newmat                                                   && build

echo "### 04. --without-log4cxx"
./configure -q                                    --without-log4cxx                                 && build

echo "### 05. --with-services"
./configure -q                                                      --with-services                 && build

echo "### 06. --without-cppunit --without-newmat"
./configure -q --without-cppunit --without-newmat                                                   && build_notest

echo "### 07. --without-cppunit --without-newmat --without-log4cxx"
./configure -q --without-cppunit --without-newmat --without-log4cxx                                 && build_notest

echo "### 08. --without-cppunit --without-newmat --without-log4cxx --with-services"
./configure -q --without-cppunit --without-newmat --without-log4cxx --with-services                 && build_notest

echo "### 09. --without-newmat --with-services"
./configure -q                   --without-newmat                   --with-services                 && build

echo "### 10. --without-newmat --without-log4cxx"
./configure -q                   --without-newmat --without-log4cxx                                 && build

echo "### 11. --without-newmat --without-log4cxx --with-services"
./configure -q                   --without-newmat --without-log4cxx --with-services                 && build

echo "### 12. --without-log4cxx --with-services"
./configure -q                                    --without-log4cxx --with-services                 && build

echo "### 13. --without-log4cxx --with-services --without-stxxl"
./configure -q                                    --without-log4cxx --with-services --without-stxxl && build

echo "### 14. ./configure -q"
./configure -q

echo "### 15. --without-cppunit --without-newmat --without-log4cxx --with-services --without-stxxl --with-rnd"
./configure -q --without-cppunit --without-newmat --without-log4cxx --with-services --without-stxxl --with-rnd && build_notest

echo "### 16. --without-newmat --without-log4cxx --with-services --without-stxxl --with-rnd"
./configure -q                   --without-newmat --without-log4cxx --with-services --without-stxxl --with-rnd && build

echo "### 17. --with-rnd"
./configure -q                                                                                      --with-rnd && build

echo "############"
echo "# Success! #"
echo "############"
exit 0
