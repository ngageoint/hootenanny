FROM rockylinux:9

LABEL maintainer="Hootenanny Development Team"
LABEL description="Rocky Linux 9 RPM build environment for Hootenanny releases"

# Install EPEL and development tools
RUN dnf install -y epel-release && \
    dnf install -y dnf-plugins-core && \
    dnf config-manager --set-enabled crb && \
    dnf groupinstall -y "Development Tools" && \
    dnf install -y \
        rpm-build \
        rpmdevtools \
        rpmlint \
        createrepo \
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
        sudo && \
    dnf clean all

# Install Java 11 (required for Hootenanny)
RUN dnf install -y java-11-openjdk java-11-openjdk-devel && \
    dnf clean all

# Set up RPM build environment
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

