#!/usr/bin/env bash
set -e

# Main sonar scanner
#SONAR_VERSION=4.0.0.1744
#SONAR_VERSION=4.1.0.1829
SONAR_VERSION=4.2.0.1873
#SONAR_VERSION=4.3.0.2102  # Broken
#SONAR_VERSION=4.4.0.2170
#SONAR_VERSION=4.5.0.2216
#SONAR_VERSION=4.6.0.2311 # Requires Java 11
SONAR_CLI=sonar-scanner-cli-$SONAR_VERSION-linux
SONAR_PKG=sonar-scanner-$SONAR_VERSION-linux
SONAR_ZIP=$SONAR_CLI.zip
SONAR_URL=https://binaries.sonarsource.com/Distribution/sonar-scanner-cli
SONAR_PATH=/opt/sonar

# Sonar build wrapper (required to scan C/C++)
SONAR_BLD_PKG=build-wrapper-linux-x86
SONAR_BLD_ZIP=$SONAR_BLD_PKG.zip
SONAR_BLD_URL=https://sonarcloud.io/static/cpp

# Download sonar scanner
if [ ! -f $SONAR_ZIP ]; then
    echo "### Downloading Sonar Scanner..."
    wget --quiet $SONAR_URL/$SONAR_ZIP
fi

# Install sonar scanner
if [ ! -f $SONAR_PATH ]; then
    echo "### Installing Sonar Scanner..."
    if [ ! -f $SONAR_PKG ]; then
        unzip -qq -o $SONAR_ZIP
    fi
    sudo mv $SONAR_PKG $SONAR_PATH
fi
echo "export PATH=\$PATH:$SONAR_PATH/bin" >> ~/.bash_profile
source ~/.bash_profile

# Download sonar build wrapper
if [ ! -f $SONAR_BLD_ZIP ]; then
    echo "### Downloading Sonar Build Wrapper..."
    wget --quiet $SONAR_BLD_URL/$SONAR_BLD_ZIP
fi

# Install sonar build wrapper
if [ ! -f $SONAR_PATH/bin/$SONAR_BLD_PKG-64 ]; then
    echo "### Installing Sonar Build Wrapper..."
    if [ ! -f $SONAR_BLD_PKG ]; then
        unzip -qq -o $SONAR_BLD_ZIP
    fi
    sudo mv $SONAR_BLD_PKG/$SONAR_BLD_PKG-64 $SONAR_PATH/bin/
    sudo mv $SONAR_BLD_PKG/libinterceptor-x86_64.so $SONAR_PATH/bin/
    sudo mv $SONAR_BLD_PKG/libinterceptor-i686.so $SONAR_PATH/bin/
    sudo mv $SONAR_BLD_PKG/libinterceptor-haswell.so $SONAR_PATH/bin/
    rm -rf $SONAR_BLD_PKG
fi

# Display the installed sonar version
sonar-scanner --version
