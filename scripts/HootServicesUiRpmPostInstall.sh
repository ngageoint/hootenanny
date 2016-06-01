#!/bin/sh
echo "Hoot Services-UI RPM PostInstall"
# init and start Postgres
PG_SERVICE=$(ls /etc/init.d | grep postgresql-)
sudo service $PG_SERVICE initdb
sudo service $PG_SERVICE start
PG_VERSION=$(sudo -u postgres psql -c 'SHOW SERVER_VERSION;' | egrep -o '[0-9]{1,}\.[0-9]{1,}')

sudo service tomcat6 start

# create Hoot services db
if ! sudo -u postgres psql -lqt | cut -d \| -f 1 | grep -qw hoot; then
    RAND_PW=$(pwgen -s 16 1)
    sudo -u postgres createuser --superuser hoot || true
    sudo -u postgres psql -c "alter user hoot with password '$RAND_PW';"
    sudo sed -i s/DB_PASSWORD=.*/DB_PASSWORD=$RAND_PW/ /var/lib/hootenanny/conf/DatabaseConfigDefault.sh
    sudo -u postgres createdb hoot --owner=hoot
    sudo -u postgres createdb wfsstoredb --owner=hoot
    sudo -u postgres psql -d hoot -c 'create extension hstore;'
    sudo -u postgres psql -d postgres -c "UPDATE pg_database SET datistemplate='true' WHERE datname='wfsstoredb'"
    sudo -u postgres psql -d wfsstoredb -c 'create extension postgis;'
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on geometry_columns TO PUBLIC;"
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on geography_columns TO PUBLIC;"
    sudo -u postgres psql -d wfsstoredb -c "GRANT ALL on spatial_ref_sys TO PUBLIC;"
fi

# configure Postgres settings
PG_HB_CONF=/var/lib/pgsql/$PG_VERSION/data/pg_hba.conf
if ! sudo grep -i --quiet hoot $PG_HB_CONF; then
    sudo -u postgres cp $PG_HB_CONF $PG_HB_CONF.orig
    sudo -u postgres sed -i '1ihost    all            hoot            127.0.0.1/32            md5' $PG_HB_CONF
    sudo -u postgres sed -i '1ihost    all            hoot            ::1/128                 md5' $PG_HB_CONF
fi
POSTGRES_CONF=/var/lib/pgsql/$PG_VERSION/data/postgresql.conf
if ! grep -i --quiet HOOT $POSTGRES_CONF; then
    sudo -u postgres cp $POSTGRES_CONF $POSTGRES_CONF.orig
    sudo -u postgres sed -i s/^max_connections/\#max_connections/ $POSTGRES_CONF
    sudo -u postgres sed -i s/^shared_buffers/\#shared_buffers/ $POSTGRES_CONF
    sudo -u postgres bash -c "cat >> $POSTGRES_CONF" <<EOT
#--------------
# Hoot Settings
#--------------
max_connections = 1000
shared_buffers = 1024MB
max_files_per_process = 1000
work_mem = 16MB
maintenance_work_mem = 256MB
checkpoint_segments = 20
autovacuum = off
EOT
fi
# configure kernel parameters
SYSCTL_CONF=/etc/sysctl.conf
if ! grep --quiet 1173741824 $SYSCTL_CONF; then
    sudo cp $SYSCTL_CONF $SYSCTL_CONF.orig
    echo "Setting kernel.shmmax"
    sudo sysctl -w kernel.shmmax=1173741824
    sudo sh -c "echo 'kernel.shmmax=1173741824' >> $SYSCTL_CONF"
    #                 kernel.shmmax=68719476736
fi
if ! grep --quiet 2097152 $SYSCTL_CONF; then
    echo "Setting kernel.shmall"
    sudo sysctl -w kernel.shmall=2097152
    sudo sh -c "echo 'kernel.shmall=2097152' >> $SYSCTL_CONF"
    #                 kernel.shmall=4294967296
fi
sudo service postgresql-$PG_VERSION restart

# create the osm api test db
sudo -i /var/lib/hootenanny/scripts/SetupOsmApiDB.sh

# configure Tomcat environment
TOMCAT_ENV=/usr/sbin/tomcat6
if ! grep -i --quiet HOOT $TOMCAT_ENV; then
    sudo cp $TOMCAT_ENV $TOMCAT_ENV.orig
    echo "Configuring tomcat6 environment"
    sudo bash -c "cat >> $TOMCAT_ENV" <<EOT
#--------------
# Hoot Settings
#--------------
HOOT_HOME=/var/lib/hootenanny
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:$HOOT_HOME/lib:$HOOT_HOME/pretty-pipes/lib
GDAL_DATA=/usr/local/share/gdal
GDAL_LIB_DIR=/usr/local/lib
HOOT_WORKING_NAME=hoot
QT_HOME=/usr/local/qt-everywhere-opensource-src-4.8.6
PSQL_HOME=/usr/pgsql-$PG_VERSION
PATH=$HOOT_HOME/bin:/usr/local/lib:/usr/local/bin:$PATH:$QT_HOME/bin:$PSQL_HOME/bin
EOT
fi
# Create Tomcat context path for tile images
TOMCAT_SRV=/etc/tomcat6/server.xml
if ! grep -i --quiet 'ingest/processed' $TOMCAT_SRV; then
    sudo -u tomcat cp $TOMCAT_SRV $TOMCAT_SRV.orig
    echo "Adding Tomcat context path for tile images"
    sudo sed -i "s@<\/Host>@ <Context docBase=\"\/var\/lib\/hootenanny\/ingest\/processed\" path=\"\/static\" \/>\n &@" $TOMCAT_SRV
fi
# Allow linking in Tomcat context
TOMCAT_CTX=/etc/tomcat6/context.xml
if ! grep -i --quiet 'allowLinking="true"' $TOMCAT_CTX; then
    sudo -u tomcat cp $TOMCAT_CTX $TOMCAT_CTX.orig
    echo "Set allowLinking to true in Tomcat context"
    sudo sed -i "s@^<Context>@<Context allowLinking=\"true\">@" $TOMCAT_CTX
fi
# Create directories for webapp
TOMCAT_HOME=/usr/share/tomcat6
if [ ! -d $TOMCAT_HOME/.deegree ]; then
    echo "Creating .deegree directory for webapp"
    sudo mkdir $TOMCAT_HOME/.deegree
    sudo chown tomcat:tomcat $TOMCAT_HOME/.deegree
fi
BASEMAP_UPLOAD_HOME=/var/lib/hootenanny/ingest/upload
if [ ! -d $BASEMAP_UPLOAD_HOME ]; then
    echo "Creating ingest/upload directory for webapp"
    sudo mkdir -p $BASEMAP_UPLOAD_HOME
    sudo chown tomcat:tomcat $BASEMAP_UPLOAD_HOME
fi
BASEMAP_PROCESSED_HOME=/var/lib/hootenanny/ingest/processed
if [ ! -d $BASEMAP_PROCESSED_HOME ]; then
    echo "Creating ingest/processed directory for webapp"
    sudo mkdir -p $BASEMAP_PROCESSED_HOME
    sudo chown tomcat:tomcat $BASEMAP_PROCESSED_HOME
fi
UPLOAD_HOME=/var/lib/hootenanny/upload
if [ ! -d $UPLOAD_HOME ]; then
    echo "Creating upload directory for webapp"
    sudo mkdir -p $UPLOAD_HOME
    sudo chown tomcat:tomcat $UPLOAD_HOME
fi
CUSTOMSCRIPT_HOME=/var/lib/hootenanny/customscript
if [ ! -d $CUSTOMSCRIPT_HOME ]; then
    echo "Creating customscript directory for webapp"
    sudo mkdir -p $CUSTOMSCRIPT_HOME
    sudo chown tomcat:tomcat $CUSTOMSCRIPT_HOME
fi
sudo chmod 777 /var/lib/hootenanny/tmp

# Update the db password in hoot-services war
source /var/lib/hootenanny/conf/DatabaseConfig.sh
while [ ! -f /var/lib/tomcat6/webapps/hoot-services/WEB-INF/classes/db/spring-database.xml ]; do
    echo "Waiting for hoot-services.war to deploy"
    sleep 1
done
sudo sed -i s/password\:\ hoottest/password\:\ $DB_PASSWORD/ /var/lib/tomcat6/webapps/hoot-services/WEB-INF/classes/db/liquibase.properties
sudo sed -i s/value=\"hoottest\"/value=\"$DB_PASSWORD\"/ /var/lib/tomcat6/webapps/hoot-services/WEB-INF/classes/db/spring-database.xml
sudo sed -i s/dbPassword=hoottest/dbPassword=$DB_PASSWORD/ /var/lib/tomcat6/webapps/hoot-services/WEB-INF/classes/conf/hoot-services.conf
sudo sed -i s/\<Password\>hoottest\<\\/Password\>/\<Password\>$DB_PASSWORD\<\\/Password\>/ /var/lib/tomcat6/webapps/hoot-services/WEB-INF/workspace/jdbc/WFS_Connection.xml
sudo sed -i s/\<jdbcPassword\>hoottest\<\\/jdbcPassword\>/\<jdbcPassword\>$DB_PASSWORD\<\\/jdbcPassword\>/ /var/lib/tomcat6/webapps/hoot-services/META-INF/maven/hoot/hoot-services/pom.xml

sudo service tomcat6 restart

# Apply any database schema changes
cd $TOMCAT_HOME/webapps/hoot-services/WEB-INF
liquibase --contexts=default,production \
    --changeLogFile=classes/db/db.changelog-master.xml \
    --promptForNonLocalDatabase=false \
    --driver=org.postgresql.Driver \
    --url=jdbc:postgresql:$DB_NAME \
    --username=$DB_USER \
    --password=$DB_PASSWORD \
    --logLevel=warning \
    --classpath=lib/postgresql-9.1-901-1.jdbc4.jar \
    update

# Configuring firewall
if ! sudo iptables --list-rules | grep -i --quiet 'dport 80'; then
    sudo iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 80 -j ACCEPT
    sudo iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 8080 -j ACCEPT
    sudo iptables -A INPUT -p tcp -m state --state NEW -m tcp --dport 8000 -j ACCEPT
    sudo iptables -A PREROUTING -t nat -p tcp --dport 80 -j REDIRECT --to-ports 8080
    sudo iptables -A OUTPUT -t nat -s 0/0 -d 127/8 -p tcp --dport 80 -j REDIRECT --to-ports 8080
    sudo service iptables save
    sudo service iptables restart
fi
