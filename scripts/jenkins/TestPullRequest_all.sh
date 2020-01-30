#!/bin/bash
# Runs all tests that are appropriate for a pull request

set -x
set -e

cd $HOOT_HOME

hoot version --debug

export HOOT_TEST_DIFF=--diff
time -p make -sj`nproc` test-all

time -p make -sj`nproc` archive
