#!/usr/bin/env bash

sudo service tomcat8 stop

sudo rm -rf /usr/share/tomcat8
sudo rm -rf /var/lib/tomcat8
sudo rm -rf /var/cache/tomcat8
sudo rm -rf /var/log/tomcat8
sudo rm -rf /etc/tomcat8

sudo update-rc.d -f tomcat8 remove

