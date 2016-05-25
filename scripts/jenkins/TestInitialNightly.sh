#!/bin/bash
# Runs the initial nightly tests

set -x
set -e

$HOOT_HOME/scripts/jenkins/TestPullRequest.sh

make -sj`nproc` coverage

