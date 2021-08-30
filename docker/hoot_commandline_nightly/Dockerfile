FROM centos:7

LABEL \
description="Container with user for running command line Hootenanny" \
maintainer="matthew.jackson@maxar.com" \
name="Hootenanny command line environment from nightly RPM" \
vendor="Maxar"


ENV container docker \
 ENV LANG en_US.UTF-8 \
 ENV LC_ALL en_US.UTF-8

# A bit overkill but allows it to be changed during the build
ARG DEV_NAME="dev"
ARG DEV_UID="1000"
ARG DEV_GID="1000"

RUN yum install -y wget curl sudo vim yum-utils epel-release && \
 yum-config-manager --add-repo https://hoot-repo.s3.amazonaws.com/el7/master/hoot.repo && \
 yum-config-manager --add-repo https://geoint-deps.s3.amazonaws.com/el7/stable/geoint-deps.repo

# Installing this to make sure that the one in the Postgresql repo DOES NOT get installed
RUN yum install -y CGAL

RUN yum -y install https://download.postgresql.org/pub/repos/yum/reporpms/EL-7-x86_64/pgdg-redhat-repo-latest.noarch.rpm

# RUN yum -y update && \
RUN yum install -y hootenanny-core

RUN yum -y clean all && rm -rf /var/cache/yum

# Add a user with sudo access
RUN useradd -mUl -G wheel -u $DEV_UID -s /bin/bash $DEV_NAME \
  && echo "$DEV_NAME ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/$DEV_NAME \
  && chmod 0440 /etc/sudoers.d/$DEV_NAME \
  && groupmod --gid $DEV_GID $DEV_NAME

USER $DEV_NAME
WORKDIR /home/$DEV_NAME

