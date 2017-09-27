#!/usr/bin/env bash

sudo systemctl stop tomcat8

sudo rm -rf /usr/share/tomcat8
sudo rm -rf /var/lib/tomcat8
sudo rm -rf /var/cache/tomcat8
sudo rm -rf /var/log/tomcat8
sudo rm -rf /etc/tomcat8


