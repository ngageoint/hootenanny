#!/bin/sh
echo "Hoot UI RPM PostUninstall"
# Stop tomcat
sudo service tomcat6 stop
# Ensure Postgres is started
PG_SERVICE=$(ls /etc/init.d | grep postgresql-)
sudo service $PG_SERVICE start
PG_VERSION=$(sudo -u postgres psql -c 'SHOW SERVER_VERSION;' | egrep -o '[0-9]{1,}\.[0-9]{1,}')
# Drop Hoot user and services db
if sudo -u postgres psql -lqt | grep -i --quiet hoot; then
    sudo -u postgres psql -d postgres -c "DROP OWNED by hoot"
    #sudo -u postgres dropdb hoot
    sudo -u postgres psql -d postgres -c "UPDATE pg_database SET datistemplate='false' WHERE datname='wfsstoredb'"
    #sudo -u postgres dropdb wfsstoredb
    sudo -u postgres dropuser hoot
fi
# configure Postgres settings
PG_HB_CONF=/var/lib/pgsql/$PG_VERSION/data/pg_hba.conf
if sudo grep -i --quiet hoot $PG_HB_CONF; then
    sudo -u postgres mv $PG_HB_CONF.orig $PG_HB_CONF
fi
POSTGRES_CONF=/var/lib/pgsql/$PG_VERSION/data/postgresql.conf
if grep -i --quiet HOOT $POSTGRES_CONF; then
    sudo -u postgres mv $POSTGRES_CONF.orig $POSTGRES_CONF
fi
# configure kernal parameters
if sysctl -e kernel.shmmax | grep --quiet 1173741824; then
    sudo mv /etc/sysctl.conf.orig /etc/sysctl.conf
fi

# configure Tomcat environment
TOMCAT_ENV=/usr/sbin/tomcat6
if grep -i --quiet HOOT $TOMCAT_ENV; then
    sudo -u tomcat mv $TOMCAT_ENV.orig $TOMCAT_ENV
fi
# Restore original file
TOMCAT_SRV=/etc/tomcat6/server.xml
if grep -i --quiet 'ingest/processed' $TOMCAT_SRV; then
    sudo -u tomcat mv $TOMCAT_SRV.orig $TOMCAT_SRV
fi
# Restore original file
TOMCAT_CTX=/etc/tomcat6/context.xml
if grep -i --quiet 'allowLinking="true"' $TOMCAT_CTX; then
    sudo -u tomcat mv $TOMCAT_CTX.orig $TOMCAT_CTX
fi
# Remove .deegree directory
TOMCAT_HOME=/usr/share/tomcat6
if [ -d $TOMCAT_HOME/.deegree ]; then
    sudo rm -rf $TOMCAT_HOME/.deegree
fi
# Remove ingest/processed directory
BASEMAP_HOME=/var/lib/hootenanny/ingest/processed
if [ -d $BASEMAP_HOME ]; then
    sudo rm -rf $BASEMAP_HOME
fi
sudo service tomcat6 start

# Configuring firewall
if sudo iptables --list-rules | grep -i --quiet 'dport 80'; then
    sudo iptables -D INPUT -p tcp -m state --state NEW -m tcp --dport 80 -j ACCEPT
    sudo iptables -D INPUT -p tcp -m state --state NEW -m tcp --dport 8080 -j ACCEPT
    sudo iptables -D INPUT -p tcp -m state --state NEW -m tcp --dport 8000 -j ACCEPT
    sudo iptables -D PREROUTING -t nat -p tcp --dport 80 -j REDIRECT --to-ports 8080
    sudo iptables -D OUTPUT -t nat -s 0/0 -d 127/8 -p tcp --dport 80 -j REDIRECT --to-ports 8080
    sudo service iptables save
    sudo service iptables restart
fi
