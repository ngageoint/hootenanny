#!/bin/bash
# Runs the initial nightly tests

set -x
set -e

cd $HOOT_HOME

hoot --version --debug

# Run the tests that don't get hit by coverage
make -sj`nproc` pp-test
make -sj`nproc` plugins-test

export HOOT_TEST_DIFF=--diff
# Running glacial tests here because we're not sure core-coverage runs them
echo "Running glacial tests..."
# Removing Poi test to troubleshoot hanging jobs on Jenkins
bin/HootTest $HOOT_TEST_DIFF --glacial --exclude=".*ConflatePoiStatsTest.*"
#bin/HootTest $HOOT_TEST_DIFF --glacial

# This is done in VagrantBuild.sh
# cd $HOOT_HOME/docs
# make -sj`nproc`

make -sj`nproc` archive

# Generate coverage reports
echo "Building core coverage reports..."
make -sj`nproc` core-coverage

echo "Building services coverage reports..."
make -sj`nproc` services-coverage
