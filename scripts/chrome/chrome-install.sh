#!/usr/bin/env bash
set -e

# Chrome has out grown CentOS 7 and can't install the current stable anymore
exit

CHROME_BASE_URL=https://dl.google.com/linux/direct
CHROME_DEB=google-chrome-stable_current_amd64.deb
CHROME_DEB_URL=$CHROME_BASE_URL/$CHROME_DEB
CHROME_RPM=google-chrome-stable_current_x86_64.rpm
CHROME_RPM_URL=$CHROME_BASE_URL/$CHROME_RPM

echo "### Installing Chrome..."
if [ "$(lsb_release -i -s)" == "Ubuntu" ]; then
    if  ! dpkg -l | grep --quiet google-chrome-stable; then
        if [ ! -f $HOME/$CHROME_DEB ]; then
            wget --quiet -O $HOME/$CHROME_DEB $CHROME_DEB_URL
        fi
        sudo apt-get -q -y install gconf-service libgconf-2-4 gconf-service-backend gconf2-common
        sudo dpkg -i $HOME/$CHROME_DEB
    fi
else
    if  ! rpm -qa | grep google-chrome-stable; then
        if [ ! -f $HOME/$CHROME_RPM ]; then
            wget --quiet -O $HOME/$CHROME_RPM $CHROME_RPM_URL
        fi
        sudo yum -y install $HOME/$CHROME_RPM
    fi
fi
