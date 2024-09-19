#!/usr/bin/env bash

export SONAR_JDK_RPM=jdk-17_linux-x64_bin.rpm
export SONAR_JDK_URL=https://download.oracle.com/java/17/latest/$SONAR_JDK_RPM
export SONAR_JDK_PATH=/usr/lib/jvm/jdk-17.0.12-oracle-x64

# Main sonar scanner
export SONAR_VERSION=5.0.1.3006 # Requires Java 17
export SONAR_CLI=sonar-scanner-cli-$SONAR_VERSION-linux
export SONAR_PKG=sonar-scanner-$SONAR_VERSION-linux
export SONAR_ZIP=$SONAR_CLI.zip
export SONAR_URL=https://binaries.sonarsource.com/Distribution/sonar-scanner-cli
export SONAR_PATH=/opt/sonar

# Sonar build wrapper (required to scan C/C++)
export SONAR_BLD_PKG=build-wrapper-linux-x86
export SONAR_BLD_ZIP=$SONAR_BLD_PKG.zip
export SONAR_BLD_URL=https://sonarcloud.io/static/cpp

# Sonar project properties
export SONAR_PROJECT_HOST_URL="https://sonarcloud.io"
export SONAR_PROJECT_ORG="hootenanny"
export SONAR_PROJECT_HOOT="hoot"
export SONAR_PROJECT_SERVICES="hoot-services"

# Sonar Maven properties
export SONAR_MVN_VERSION=apache-maven-3.9.8
export SONAR_MVN_TAR=${SONAR_MVN_VERSION}-bin.tar.gz
export SONAR_MVN_URL=https://dlcdn.apache.org/maven/maven-3/3.9.8/binaries/${SONAR_MVN_TAR}

export M2_HOME=/opt/maven
export MAVEN_HOME=/opt/maven
export PATH=${MAVEN_HOME}/bin:${PATH}

