#!/bin/bash

# create the mocha-coverage report
cd $HOOT_HOME/hoot-ui
npm install -q
npm run cover

# move istanbul instrumented hoot-ui javascript code into tomcat webapps
sudo -u tomcat6 rm -rf /var/lib/tomcat6/webapps/hootenanny-id/js/hoot
sudo -u tomcat6 rm -rf /var/lib/tomcat6/webapps/hootenanny-id/js/id
sudo -u tomcat6 HOOT_HOME=$HOOT_HOME cp -R $HOOT_HOME/hoot-ui/istanbul/hoot /var/lib/tomcat6/webapps/hootenanny-id/js/hoot
sudo -u tomcat6 HOOT_HOME=$HOOT_HOME cp -R $HOOT_HOME/hoot-ui/istanbul/id /var/lib/tomcat6/webapps/hootenanny-id/js/id

# start istanbul-middleware
cd $HOOT_HOME/test-files/ui/coverage_app
npm install -q
node app.js &
PID=$!

# run cucumber tests
cd $HOOT_HOME
make ui-test -s

# download the cucumber-coverage report
cd $HOOT_HOME/hoot-ui
mkdir -p cucumber-coverage
wget http://localhost:8880/coverage/download -O cucumber-coverage/coverage.zip -q
unzip -oq cucumber-coverage/coverage.zip -d cucumber-coverage

# generate combined mocha/cucumber report
npm run combine

# stop istanbul-middleware
kill -9 $PID

# restore uninstrumented hoot-ui code to tomcat webapps
sudo -u tomcat6 rm -rf /var/lib/tomcat6/webapps/hootenanny-id/js/hoot
sudo -u tomcat6 rm -rf /var/lib/tomcat6/webapps/hootenanny-id/js/id
sudo -u tomcat6 HOOT_HOME=$HOOT_HOME cp -R $HOOT_HOME/hoot-ui/js/hoot /var/lib/tomcat6/webapps/hootenanny-id/js/hoot
sudo -u tomcat6 HOOT_HOME=$HOOT_HOME cp -R $HOOT_HOME/hoot-ui/js/id /var/lib/tomcat6/webapps/hootenanny-id/js/id
