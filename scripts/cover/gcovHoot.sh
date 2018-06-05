#!/usr/bin/env bash
set -e

# set HOOT_HOME if not set
if [ -z "$HOOT_HOME" ]; then
    HOOT_HOME=~/hoot
fi

# generate gcov files from gcda files
cd $HOOT_HOME

# hoot-core coverage
cd hoot-core
gcov tmp/debug/*.gcda
# fix path
sed -i 's/src/hoot-core\/src/g' *.gcov
mv *.gcov ../
cd ..

#tbs coverage
cd tbs
gcov tmp/debug/*.gcda
# fix path
sed -i 's/src/tbs\/src/g' *.gcov
mv *.gcov ../
cd ..

# hoot-rnd coverage
cd hoot-rnd
gcov tmp/debug/*.gcda
# fix path
sed -i 's/src/hoot-rnd\/src/g' *.gcov
mv *.gcov ../
cd ..
