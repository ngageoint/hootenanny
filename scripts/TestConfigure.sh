#!/bin/bash

set -e

aclocal
autoconf
./configure -q && make -s clean

function build_notest {
    make -s clean
    make -sj8
    hoot --version
    # make sure hadoop is enabled
    hoot --help | grep -- big-conflate &> /dev/null
    make -s clean
}

function build {
    make -s clean
    make -sj8
    HootTest --quick
    # make sure hadoop is enabled
    hoot --help | grep -- big-conflate &> /dev/null
    make -s clean
}

# Try a lot of the configure combinations to see if there is a broken .pro file.
# Trying all of the combinations would take 32 runs.
./configure -q --without-cppunit                                                                    && build_notest
./configure -q                                                                      --without-stxxl && build
./configure -q                   --without-newmat                                                   && build
./configure -q                                    --without-log4cxx                                 && build
./configure -q                                                      --with-services                 && build
./configure -q --without-cppunit --without-newmat                                                   && build_notest
./configure -q --without-cppunit --without-newmat --without-log4cxx                                 && build_notest
./configure -q --without-cppunit --without-newmat --without-log4cxx --with-services                 && build_notest
./configure -q                   --without-newmat                   --with-services                 && build
./configure -q                   --without-newmat --without-log4cxx                                 && build
./configure -q                   --without-newmat --without-log4cxx --with-services                 && build
./configure -q                                    --without-log4cxx --with-services                 && build
./configure -q                                    --without-log4cxx --with-services --without-stxxl && build
./configure -q
./configure -q --without-cppunit --without-newmat --without-log4cxx --with-services --without-stxxl --with-rnd && build_notest
./configure -q                   --without-newmat --without-log4cxx --with-services --without-stxxl --with-rnd && build
./configure -q                                                                                      --with-rnd && build

echo "############"
echo "# Success! #"
echo "############"
exit 0
