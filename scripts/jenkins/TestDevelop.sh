#!/bin/bash
# Runs all tests that are appropriate for the Develop branch

set -x
set -e

cd $HOOT_HOME

hoot version --debug

export HOOT_TEST_DIFF=--diff
# Run all of the tests - from the "test-all" target
echo "Running core glacial tests..."
bin/HootTest $HOOT_TEST_DIFF --glacial --parallel $(nproc)

echo "Running plugins tests..."
make -sj`nproc` plugins-test
echo "Running services tests..."
make -sj`nproc` services-test
echo "Running ui tests..."
make -s ui-test
