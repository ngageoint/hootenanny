#!/usr/bin/env bash
set -e

# set HOOT_HOME if not set
if [ -z "$HOOT_HOME" ]; then
    HOOT_HOME=~/hoot
fi

# generate gcov files from gcda files
cd $HOOT_HOME

# hoot-core coverage
mkdir gcovCore
cd gcovCore
gcov ../hoot-core/tmp/debug/*.gcda
# fix path
sed -i 's/Source:src/Source:hoot-core\/src/g' *.gcov
sed -i 's/Source:..\//Source:/g' *.gcov
mv *.gcov ../
cd ..

# tbs coverage
mkdir gcovTbs
cd gcovTbs
gcov ../tbs/tmp/debug/*.gcda
# fix path
sed -i 's/Source:src/Source:tbs\/src/g' *.gcov
sed -i 's/Source:..\//Source:/g' *.gcov
mv *.gcov ../
cd ..

# tgs coverage
mkdir gcovTgs
cd gcovTgs
gcov ../tgs/tmp/obj/debug/*.gcda
# fix path
sed -i 's/Source:src/Source:tgs\/src/g' *.gcov
sed -i 's/Source:..\//Source:/g' *.gcov
mv *.gcov ../
cd ..

# hoot-rnd coverage
mkdir gcovRnd
cd gcovRnd
gcov ../hoot-rnd/tmp/debug/*.gcda
# fix path
sed -i 's/Source:src/Source:hoot-rnd\/src/g' *.gcov
sed -i 's/Source:..\//Source:/g' *.gcov
mv *.gcov ../
cd ..
