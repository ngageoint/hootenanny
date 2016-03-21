#!/bin/sh
echo "Hoot UI RPM PostUpgrade"

# Apply any database schema changes
cd $TOMCAT_HOME/webapps/hoot-services/WEB-INF
liquibase --contexts=default,production \
    --changeLogFile=classes/db/db.changelog-master.xml \
    --promptForNonLocalDatabase=false \
    --defaultsFile=classes/db/liquibase.properties \
    --logLevel=warning \
    --classpath=lib/postgresql-9.1-901-1.jdbc4.jar \
    --url jdbc:postgresql:hoot update

