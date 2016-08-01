#!/bin/bash
# Runs all tests that are appropriate for a pull request

set -x
set -e

cd $HOOT_HOME

hoot --version --debug

time -p make -s ui-test
