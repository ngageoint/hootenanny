#!/bin/bash

#tested on CentOS 6.6

sudo yum update
hootenanny-X.Y.Z/scripts/InstallHootDependencies-CentOS-RHEL-Full-EXPERIMENTAL.sh

HOOT_VERSION=X.Y.Z
cd ~
wget https://github.com/ngageoint/hootenanny/releases/download/v$HOOT_VERSION/hootenanny-$HOOT_VERSION.tar.gz
wget https://github.com/ngageoint/hootenanny/releases/download/v$HOOT_VERSION/hootenanny-services-$HOOT_VERSION.war
tar -xzf hootenanny-$HOOT_VERSION.tar.gz
hootenanny-X.Y.Z/scripts/UpdateHoot-CentOS-RHEL.sh


