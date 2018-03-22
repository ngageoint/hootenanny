#!/bin/bash
# Runs all tests that are appropriate for a pull request

set -x
set -e

cd $HOOT_HOME

hoot version --debug

time -p make -j`nproc` test-all-core

# This is done in VagrantBuild.sh
# cd $HOOT_HOME/docs
# make -sj`nproc`

time -p make -sj`nproc` archive
