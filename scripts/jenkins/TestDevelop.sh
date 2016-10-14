#!/bin/bash
# Runs all tests that are appropriate for the Develop branch

set -x
set -e

cd $HOOT_HOME

hoot --version --debug

export HOOT_TEST_DIFF=--diff
# Run all of the tests - from the "test-all" target
echo "Running core glacial tests..."
# Removing Poi test to troubleshoot hanging jobs on Jenkins
bin/HootTest $HOOT_TEST_DIFF --glacial --exclude=".*ConflatePoiStatsTest.*"
#bin/HootTest $HOOT_TEST_DIFF --glacial


echo "Running pretty-pipes tests..."
make -sj`nproc` pp-test
echo "Running plugins tests..."
make -sj`nproc` plugins-test
echo "Running services tests..."
make -sj`nproc` services-test
echo "Running ui tests..."
make -s ui-test
