#!/bin/sh
echo "Hoot Services-UI RPM PostUpgrade"

# Apply any database schema changes
TOMCAT_HOME=/usr/share/tomcat6
source /var/lib/hootenanny/conf/DatabaseConfig.sh
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
