#!/bin/bash
# Runs the initial nightly tests

set -x
set -e

cd $HOOT_HOME

hoot --version --debug

export HOOT_TEST_DIFF=--diff
make -sj`nproc` test-all

# This is done in VagrantBuild.sh
# cd $HOOT_HOME/docs
# make -sj`nproc`

make -sj`nproc` archive

make -sj`nproc` coverage

