#!/usr/bin/env bash
set -e

# set HOOT_HOME if not set
if [ -z "$HOOT_HOME" ]; then
    HOOT_HOME=~/hoot
fi

# generate gcov files from gcda files
cd $HOOT_HOME

# hoot-core coverage
mkdir gcov-hoot-core
cd gcov-hoot-core
gcov ../hoot-core/tmp/debug/*.gcda
# fix path
sed -i 's/src/hoot-core\/src/g' *.gcov
mv *.gcov ../
cd ..

#tbs coverage
mkdir gcov-tbs
cd gcov-tbs
gcov ../tbs/tmp/debug/*.gcda
# fix path
sed -i 's/src/tbs\/src/g' *.gcov
mv *.gcov ../
cd ..

# hoot-rnd coverage
mkdir gcov-hoot-rnd
cd gcov-hoot-rnd
gcov ../hoot-rnd/tmp/debug/*.gcda
# fix path
sed -i 's/src/hoot-rnd\/src/g' *.gcov
mv *.gcov ../
cd ..
