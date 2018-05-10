#!/usr/bin/env bash
set -e

export OS_NAME="$(lsb_release -i -s)"

if [ "$OS_NAME" == "Ubuntu" ]; then
  source ~/.profile
else # Centos
  source ~/.bash_profile
fi

if [ -z "$HOOT_HOME" ]; then
    HOOT_HOME=~/hoot
fi
echo HOOT_HOME: $HOOT_HOME

cd $HOOT_HOME
source ./SetupEnv.sh

echo "### Configuring Hoot..."
echo HOOT_HOME: $HOOT_HOME

# Going to remove this so that it gets updated
if [ -f missing ]; then
  rm -f missing
fi

# Configure the make
aclocal && autoconf && autoheader && automake --add-missing --copy
./configure --quiet --with-rnd --with-services --with-uitests

# Make with the sonar build watcher
build-wrapper-linux-x86-64 --out-dir bw-output make -sj$(nproc)

# Perform scan and upload to sonar scanner
sonar-scanner \
    -Dsonar.projectKey=hoot \
    -Dsonar.sources=./hoot-cmd,./hoot-core,./hoot-core-test,./hoot-js,./hoot-rnd,./hoot-test,./tbs,./tgs \
    -Dsonar.cfamily.build-wrapper-output=bw-output \
    -Dsonar.host.url=https://sonarcloud.io \
    -Dsonar.organization=hootenanny \
    -Dsonar.login=$1 \
    -Dsonar.cfamily.threads=4