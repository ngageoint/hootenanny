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

GCOV_SCAN_PATHS = ( "hoot-core" "tbs" "tgs" "hoot-cmd" "hoot-js" "hoot-josm" )

for GCOV_PATH in ${GCOV_SCAN_PATHS[@]}; do
    pushd $GCOV_PATH
    # Create coverage
    gcov --quiet -o $HOOT_HOME/$GCOV_PATH/tmp/debug/ $HOOT_HOME/$GCOV_PATH/tmp/debug/*.gcda
    # Fix path
    sed -i "s|0:Source:src|0:Source:$HOOT_HOME/$GCOV_PATH/src|g" *.gcov
    mv *.gcov $HOOT_HOME/gcov
    popd
done

# fix final paths
sed -i "s|0:Source:../|0:Source:|g" gcov/*.gcov
sed -i '/0:Source:\//!s/0:Source:/0:Source:HOOT_HOME\//g' gcov/*.gcov
sed -i "s|HOOT_HOME|$HOOT_HOME|g" gcov/*.gcov
# remove any files that aren't part of hootenanny code
pushd gcov
grep -Lr "$HOOT_HOME" *.gcov | xargs rm -f
popd
