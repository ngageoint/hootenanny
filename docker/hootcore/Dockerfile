FROM centos:7

LABEL \
 description="Container for Hootenanny in GBDX" \
 maintainer="matthew.jackson@digitalglobe.com" \
 name="GBDX Hootenanny" \
 vendor="Radiant Solutions"

ENV container docker
ENV LANG en_US.UTF-8
ENV LC_ALL en_US.UTF-8

RUN localedef --quiet -c -i en_US -f UTF-8 en_US.UTF-8

# Setup the epel, postgres and Hoot repos
ADD hoot_etc.tar.gz /

RUN chown root:root /etc/yum.repos.d/hoot.repo \
 && chmod 0644 /etc/yum.repos.d/hoot.repo \
 && chown root:root /etc/pki/rpm-gpg/RPM-GPG-KEY-Hoot \
 && chmod 0644 /etc/pki/rpm-gpg/RPM-GPG-KEY-Hoot \
 && yum -y install epel-release \
 && rpm -Uvh https://download.postgresql.org/pub/repos/yum/9.5/redhat/rhel-7-x86_64/pgdg-centos95-9.5-3.noarch.rpm \
 && yum -q -y update

# Install the NodeSource repo and an ancient version of NodeJS
RUN curl --silent --location https://rpm.nodesource.com/setup | bash -
RUN yum install -q -y nodejs-0.10.48 nodejs-devel-0.10.48 yum-plugin-versionlock \
 && yum versionlock nodejs*


# Install all the stuff we need to build a minimal version of Hoot
RUN yum install -q -y \
    autoconf \
    automake \
    bc \
    boost-devel \
    bzip2 \
    cmake \
    cppunit-devel \
    curl \
    file \
    gcc \
    gcc-c++ \
    gdb \
    geos \
    geos-devel \
    git \
    git-core \
    glpk \
    glpk-devel \
    hoot-gdal \
    hoot-gdal-devel \
    hoot-gdal-python \
    libicu-devel \
    liboauthcpp \
    liboauthcpp-devel \
    libpng-devel \
    libtool \
    mlocate \
    opencv \
    opencv-core \
    opencv-devel \
    opencv-python \
    osmosis \
    java-1.8.0-openjdk \
    perl-XML-LibXML \
    hoot-postgis24_95 \
    postgresql95 \
    postgresql95-contrib \
    postgresql95-devel \
    postgresql95-server \
    proj \
    proj-devel \
    protobuf \
    protobuf-compiler \
    protobuf-devel \
    python-devel \
    python-matplotlib \
    python-pip  \
    python-setuptools \
    qt \
    qt-devel \
    qt-postgresql \
    qtwebkit \
    qtwebkit-devel \
    redhat-lsb-core \
    stxxl \
    stxxl-devel \
    swig \
    unzip \
    v8-devel \
    vim \
    wamerican-insane \
    w3m \
    wget \
    words \
    zip


# Make sure that we have qmake for the build.
RUN alternatives --install /usr/bin/qmake qmake /usr/bin/qmake-qt5 500

# Cleanup
RUN rm -f /etc/udev/rules.d/70-persistent-net.rules /var/log/wtmp /var/log/btmp \
 && rm -rf /tmp/* \
 && yum clean all
