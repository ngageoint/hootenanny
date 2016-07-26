#!/bin/bash
# Runs the initial nightly tests

set -x
set -e

cd $HOOT_HOME

hoot --version --debug

export HOOT_TEST_DIFF=--diff

# Run all of the tests - fronm the "test-all" target
make -sj`nproc` services-test-all
make -sj`nproc` pp-test
make -sj`nproc` plugins-test
make -s ui-test

echo "Running glacial tests..."
bin/HootTest $(HOOT_TEST_DIFF) --glacial


# This is done in VagrantBuild.sh
# cd $HOOT_HOME/docs
# make -sj`nproc`

make -sj`nproc` archive

# Generate coverage reports
echo "Building core coverage reports..."
make -sj`nproc` core-coverage

echo "Building services coverage reports..."
make -sj`nproc` services-coverage

echo "Building UI coverage reports..."
make -sj`nproc` ui-coverage

