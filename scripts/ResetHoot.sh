#!/bin/bash

# This simple script is meant only as a workaround to reset the hoot server to a usable state onsite
# when it becomes unusable.  Eventuallly, with continuous onsite support this won't be needed.  
# The script has been tested on CentOS 6.5.

sudo service postgresql-9.1 restart
sleep 10
sudo service tomcat8 restart
sleep 10

echo "The Hootenanny server has been reset."
