#!/usr/bin/env bash
set -e

# Configure the make
aclocal && autoconf && autoheader && automake --add-missing --copy
./configure --quiet --with-rnd --with-services --with-uitests

# Make with the sonar build watcher
build-wrapper-linux-x86-64 --out-dir bw-output make -sj$(nproc)

# Build out the scan command
CMD="sonar-scanner"
CMD+=" -Dsonar.projectKey=hoot"
CMD+=" -Dsonar.sources=./hoot-cmd,./hoot-core,./hoot-core-test,./hoot-js,./hoot-rnd,./hoot-test,./tbs,./tgs"
CMD+=" -Dsonar.cfamily.build-wrapper-output=bw-output"
CMD+=" -Dsonar.host.url=https://sonarcloud.io"
CMD+=" -Dsonar.organization=hootenanny"
CMD+=" -Dsonar.cfamily.threads=4"
CMD+=" -Dsonar.exclusions=**/*.pb.cc,**/*.pb.h,**/*.sql"
CMD+=" -Dsonar.cfamily.lcov.reportsPaths=./coverage/core/core/Core.info"
CMD+=" -Dsonar.github.repository=ngageoint/hootenanny"

# Optional scan parameters based off parameters passed into script
if [ -n "$1" ]; then
    # This is the hoot sonarcloud project key, requried to pass scan results to sever for analysis and display
    CMD+=" -Dsonar.login=$1"
fi
if [ -n "$2" ]; then
    CMD+=" -Dsonar.branch.name=$2"
fi
if [ -n "$3" ]; then
    # Optional pull request number that will match scan with git hub pull-request
    CMD+=" -Dsonar.github.pullRequest=$3"
    CMD+=" -Dsonar.analysis.mode=preview"
fi
if [ -n "$4" ]; then
    # Optional token to allow scan to post comments to github ticket
    CMD+=" -Dsonar.github.oauth=$4"
fi
 
eval $CMD
