#!/usr/bin/env bash

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root"
   exit 1
fi

service tomcat8 stop

rm -rf /usr/share/tomcat8
rm -rf /var/lib/tomcat8
rm -rf /var/cache/tomcat8
rm -rf /var/log/tomcat8
rm -rf /etc/tomcat8

update-rc.d -f tomcat8 remove