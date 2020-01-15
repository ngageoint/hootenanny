#!/usr/bin/env bash
set -e
# set HOOT_HOME if not set
if [ -z "$HOOT_HOME" ]; then
    HOOT_HOME=~/hoot
fi
# generate gcov files from gcda files
cd $HOOT_HOME
# create the gcov output folder
if [ ! -d gcov ]; then
    mkdir gcov
else
    rm -f gcov/*
fi
# hoot-core coverage
pushd hoot-core
gcov -o $HOOT_HOME/hoot-core/tmp/debug/ $HOOT_HOME/hoot-core/tmp/debug/*.gcda
# fix path
sed -i "s|0:Source:src|0:Source:$HOOT_HOME/hoot-core/src|g" *.gcov
mv *.gcov $HOOT_HOME/gcov
popd
# tbs coverage
pushd tbs
gcov -o $HOOT_HOME/tbs/tmp/debug/ $HOOT_HOME/tbs/tmp/debug/*.gcda
# fix path
sed -i "s|0:Source:src|0:Source:$HOOT_HOME/tbs/src|g" *.gcov
mv *.gcov $HOOT_HOME/gcov
popd
# tgs coverage
pushd tgs
gcov -o $HOOT_HOME/tgs/tmp/obj/debug/ $HOOT_HOME/tgs/tmp/obj/debug/*.gcda
# fix path
sed -i "s|0:Source:src|0:Source:$HOOT_HOME/tgs/src|g" *.gcov
mv *.gcov $HOOT_HOME/gcov
popd
# hoot-rnd coverage
pushd hoot-rnd
gcov -o $HOOT_HOME/hoot-rnd/tmp/debug/ $HOOT_HOME/hoot-rnd/tmp/debug/*.gcda
# fix path
sed -i "s|0:Source:src|0:Source:$HOOT_HOME/hoot-rnd/src|g" *.gcov
mv *.gcov $HOOT_HOME/gcov
popd
# fix final paths
sed -i "s|0:Source:../|0:Source:|g" gcov/*.gcov
sed -i '/0:Source:\//!s/0:Source:/0:Source:HOOT_HOME\//g' gcov/*.gcov
sed -i "s|HOOT_HOME|$HOOT_HOME|g" gcov/*.gcov
# remove any files that aren't part of hootenanny code
pushd gcov
grep -Lr "$HOOT_HOME" *.gcov | xargs rm -f
popd
