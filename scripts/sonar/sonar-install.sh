#!/usr/bin/env bash
set -e

# Main sonar scanner
SONAR_VERSION=4.6.0.2311 # Requires Java 11
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

# Install Java 11 for the scan only
sudo yum install -y java-11-openjdk java-11-openjdk-devel

# Display the installed sonar version
JAVA_HOME=/usr/lib/jvm/java-11-openjdk sonar-scanner --version

# In order to run correctly in sonar, pre-compiled headers need to be turned off
# Remove when https://jira.sonarsource.com/browse/CPP-2897 is resolved
for FILENAME in \
  "hoot-core/hoot-core.pro" \
  "hoot-core-test/hoot-core-test.pro" \
  "hoot-josm/hoot-josm.pro" \
  "hoot-js/hoot-js.pro" \
  "hoot-rnd/hoot-rnd.pro" \
  "hoot-swig/hoot-swig.pro" \
  "tgs/src/test/cpp/tgs/TgsTest.pro" \
  "tgs/src/test/cpp/tgs/TgsTestLib.pro" \
  "tgs/tgs.pro"
do
  sed -i "s/^PRECOMPILED_HEADER.*$//g" $FILENAME
done
