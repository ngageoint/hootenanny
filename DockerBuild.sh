#!/bin/bash
#must be run before trying to do vagrant/docker/centos combination
sudo docker pull centos:centos7.4.1708
sudo docker pull centos:centos7.2.1511
sudo docker build --rm -t local/centos7.4vagrant ./docker/centos7.4/
sudo docker build --rm -t local/centos7.2vagrant ./docker/centos7.2/
