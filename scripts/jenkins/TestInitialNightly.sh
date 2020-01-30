#!/bin/bash
# Runs the initial nightly tests

set -x
set -e

cd $HOOT_HOME

hoot version --debug

# Run the tests that don't get hit by coverage
make -sj`nproc` translations-test

export HOOT_TEST_DIFF=--diff
# Running glacial tests here because we're not sure core-coverage runs them
echo "Running glacial tests..."
bin/HootTest $HOOT_TEST_DIFF --glacial --parallel

make -sj`nproc` archive

# Generate coverage reports
echo "Building core coverage reports..."
make -sj`nproc` core-coverage

echo "Building services coverage reports..."
make -sj`nproc` services-coverage
