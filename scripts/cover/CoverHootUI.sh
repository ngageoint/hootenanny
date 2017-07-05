#!/bin/bash

export TOMCAT8_HOME=/usr/share/tomcat8
# create the mocha-coverage report
cd $HOOT_HOME/hoot-ui
npm install -q
npm run cover

# move istanbul instrumented hoot-ui javascript code into tomcat webapps
sudo service tomcat8 stop
sudo -u tomcat8 rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/
sudo -u tomcat8 cp -R $HOOT_HOME/hoot-ui $TOMCAT8_HOME/webapps/hootenanny-id
sudo -u tomcat8 HOOT_HOME=$HOOT_HOME cp -R $HOOT_HOME/hoot-ui/istanbul/hoot /$TOMCAT8_HOME/webapps/hootenanny-id/js/hoot
sudo -u tomcat8 HOOT_HOME=$HOOT_HOME cp -R $HOOT_HOME/hoot-ui/istanbul/id $TOMCAT8_HOME/webapps/hootenanny-id/js/id
# activate EGD plugin so it's available for cucumber tests
[ -f ~/ActivateEGDplugin.sh ] && sudo -u tomcat8 ~/ActivateEGDplugin.sh $TOMCAT8_HOME

sudo service tomcat8 start

# start istanbul-middleware
cd $HOOT_HOME/test-files/ui/coverage_app
npm install -q
node app.js &
PID=$!

# run cucumber tests
cd $HOOT_HOME/test-files/ui
make test -s

# download the cucumber-coverage report
cd $HOOT_HOME/hoot-ui
mkdir -p cucumber-coverage
wget http://localhost:8880/coverage/download -O cucumber-coverage/coverage.zip -q
unzip -oq cucumber-coverage/coverage.zip -d cucumber-coverage

# generate combined mocha/cucumber report
npm run combine

# stop istanbul-middleware
kill $PID

# restore uninstrumented hoot-ui code to tomcat webapps
sudo service tomcat8 stop
sudo -u tomcat8 rm -rf $TOMCAT8_HOME/webapps/hootenanny-id/
sudo -u tomcat8 cp -R $HOOT_HOME/hoot-ui/dist $TOMCAT8_HOME/webapps/hootenanny-id
sudo -u tomcat8 mkdir -p $TOMCAT8_HOME/webapps/hootenanny-id/data
sudo -u tomcat8 cp $HOOT_HOME/hoot-ui/data/osm-plus-taginfo.csv $TOMCAT8_HOME/webapps/hootenanny-id/data
sudo -u tomcat8 cp $HOOT_HOME/hoot-ui/data/tdsv61_field_values.json $TOMCAT8_HOME/webapps/hootenanny-id/data
sudo service tomcat8 start