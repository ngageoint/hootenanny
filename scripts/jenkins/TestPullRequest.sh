#!/bin/bash
# Runs all tests that are appropriate for a pull request

set -x
set -e

$HOOT_HOME/scripts/jenkins/TestAll.sh

cd $HOOT_HOME/docs
make -sj`nproc`

cd $HOOT_HOME
make -sj`nproc` archive
