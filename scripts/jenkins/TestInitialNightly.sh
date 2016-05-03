#!/bin/bash
# Runs the initial nightly tests

set -x
set -e

# For the initial test run
make -sj`nproc`

hoot --version --debug

#$HOOT_HOME/scripts/jenkins/TestPullRequest.sh

cd $HOOT_HOME/docs
make -sj`nproc`

cd $HOOT_HOME
make -sj`nproc` archive
