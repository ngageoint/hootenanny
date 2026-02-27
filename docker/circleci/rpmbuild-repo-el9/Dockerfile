FROM rockylinux:9

LABEL maintainer="Hootenanny Development Team"
LABEL description="Rocky Linux 9 RPM repository build environment for Hootenanny"

# Install EPEL and repository tools
RUN dnf install -y epel-release && \
    dnf install -y dnf-plugins-core && \
    dnf config-manager --set-enabled crb && \
    dnf install -y \
        createrepo_c \
        rpm-sign \
        gnupg2 \
        git \
        wget \
        curl \
        tar \
        gzip \
        bzip2 \
        xz \
        rsync \
        which \
        sudo \
        python3 \
        python3-pip && \
    dnf clean all

# Install additional Python tools for repository management
RUN pip3 install --no-cache-dir \
        boto3 \
        requests

# Set up repository builder user
RUN useradd -m -s /bin/bash repobuilder && \
    echo "repobuilder ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

USER repobuilder
WORKDIR /home/repobuilder

# Create directory structure for repository
RUN mkdir -p ~/repo/{RPMS,SRPMS,repodata}

USER root
WORKDIR /

CMD ["/bin/bash"]

