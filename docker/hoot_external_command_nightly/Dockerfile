FROM centos:7

LABEL \
description="Container for running a Hootenanny command via flask" \
maintainer="matthew.jackson@maxar.com" \
name="Hootenanny External Command" \
vendor="Maxar"


ENV container=docker 
ENV LANG=en_US.UTF-8 
ENV LC_ALL=en_US.UTF-8

RUN yum install -y wget curl sudo vim yum-utils epel-release && \
 yum-config-manager --add-repo https://hoot-repo.s3.amazonaws.com/el7/master/hoot.repo && \
 yum-config-manager --add-repo https://geoint-deps.s3.amazonaws.com/el7/stable/geoint-deps.repo

# Installing this to make sure that the one in the Postgresql repo DOES NOT get installed
RUN yum install -y CGAL

RUN yum -y install https://download.postgresql.org/pub/repos/yum/reporpms/EL-7-x86_64/pgdg-redhat-repo-latest.noarch.rpm

# RUN yum -y update && \
RUN yum install -y hootenanny-core
RUN pip3 install flask flask_shell2http flask-executor

# Put the entrypoint & flask scripts into the container
COPY scripts /home/scripts

ENV FLASK_APP=/home/scripts/CommandsAcrossContainer.py
ENV FLASK_ENV=development

RUN yum -y clean all && rm -rf /var/cache/yum

WORKDIR /home
ENTRYPOINT [ "/home/scripts/hoot_docker_entrypoint.sh" ]
