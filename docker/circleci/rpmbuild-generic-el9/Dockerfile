FROM rockylinux:9

LABEL maintainer="Hootenanny Development Team"
LABEL description="Rocky Linux 9 generic RPM build environment for Hootenanny"

# Install EPEL and comprehensive development tools
RUN dnf install -y epel-release && \
    dnf install -y dnf-plugins-core && \
    dnf config-manager --set-enabled crb && \
    dnf groupinstall -y "Development Tools" && \
    dnf install -y \
        rpm-build \
        rpmdevtools \
        rpmlint \
        createrepo_c \
        git \
        wget \
        curl \
        tar \
        gzip \
        bzip2 \
        xz \
        make \
        cmake \
        gcc \
        gcc-c++ \
        autoconf \
        automake \
        libtool \
        which \
        sudo \
        python3 \
        python3-devel \
        python3-pip \
        java-11-openjdk \
        java-11-openjdk-devel \
        maven \
        nodejs \
        npm && \
    dnf clean all

# Install additional build dependencies commonly needed
RUN dnf install -y \
        qt5-qtbase-devel \
        qt5-qtscript-devel \
        qt5-qtwebkit-devel \
        boost-devel \
        protobuf-devel \
        protobuf-compiler \
        opencv-devel \
        gdal-devel \
        geos-devel \
        proj-devel \
        postgresql-devel \
        libpq-devel \
        libxml2-devel \
        libxslt-devel \
        zlib-devel \
        bzip2-devel \
        openssl-devel \
        readline-devel \
        sqlite-devel && \
    dnf clean all

# Set up generic builder user
RUN useradd -m -s /bin/bash builder && \
    echo "builder ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

USER builder
WORKDIR /home/builder

# Create RPM build directory structure
RUN rpmdev-setuptree

# Set environment variables
ENV JAVA_HOME=/usr/lib/jvm/java-11-openjdk
ENV PATH=$JAVA_HOME/bin:$PATH

USER root
WORKDIR /

CMD ["/bin/bash"]

