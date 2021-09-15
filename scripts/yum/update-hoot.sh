#!/usr/bin/env bash

set -e

echo "### Updating Hootenanny-autostart ###"
sudo yum makecache -y
sudo yum update hootenanny-autostart -y
echo "### Done ###"

echo "### Configure OAuth redirect url for port 8888 ###"
sudo $HOOT_HOME/scripts/tomcat/configure_oauth_8888.sh

# Sanity check
hoot version
