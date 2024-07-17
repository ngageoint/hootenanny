#!/usr/bin/env bash
set -e

source $HOOT_HOME/scripts/sonar/sonar-config.sh

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

# Install Java 17 for the scan only
echo "before java 17 install"
wget -q $SONAR_JDK_URL
sudo yum localinstall -y -q $SONAR_JDK_RPM
echo "after java 17 install"

# Install an updated version of Maven for Sonar
wget -q --no-check-certificate $SONAR_MVN_URL
sudo tar xzf $SONAR_MVN_TAR -C /opt
sudo ln -s /opt/$SONAR_MVN_VERSION /opt/maven

# Display the installed sonar version
echo "before java home"
JAVA_HOME=$SONAR_JDK_PATH sonar-scanner --version
echo "Sonar JDK Path: $SONAR_JDK_PATH"
JAVA_HOME=$SONAR_JDK_PATH mvn --version
echo "after setting java home"

# In order to run correctly in sonar, pre-compiled headers need to be turned off
# Remove when https://jira.sonarsource.com/browse/CPP-2897 is resolved
for FILENAME in \
  "hoot-core/hoot-core.pro" \
  "hoot-core-test/hoot-core-test.pro" \
  "hoot-josm/hoot-josm.pro" \
  "hoot-js/hoot-js.pro" \
  "tgs/src/test/cpp/tgs/TgsTest.pro" \
  "tgs/src/test/cpp/tgs/TgsTestLib.pro" \
  "tgs/tgs.pro"
do
  sed -i "s/^PRECOMPILED_HEADER.*$//g" $FILENAME
done
