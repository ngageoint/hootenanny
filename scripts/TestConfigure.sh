#!/bin/bash

set -e

aclocal && autoconf && autoheader && automake --add-missing && ./configure -q && make -s clean

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

echo "### 15. --without-cppunit --without-newmat --without-log4cxx --with-services --without-stxxl --with-rnd"
./configure -q --without-cppunit --without-newmat --without-log4cxx --with-services --without-stxxl --with-rnd && build_notest

echo "############"
echo "# Success! #"
echo "############"
exit 0
