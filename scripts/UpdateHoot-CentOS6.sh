#!/bin/bash

# This script: 
# - installs a new or updated instance of Hootenanny to an environment as defined in 
# $HOOT_HOME/docs/Hootenanny - Installation Instructions.pdf
# - assumes all Hootenanny dependencies have previously been installed
# - assumes JAVA_HOME has already been set 
# - applies to the production CentOS 6.x environment only
# - *does not* back out of the entire installation process if any errors occur during the 
# installation
#
# WARNING: This script is very dependent on the Hootenanny installation process as defined in the
# instructions referenced above.  Spend time to understand what this script is doing before 
# attempting to execute it.  USE THIS SCRIPT AT YOUR OWN RISK.
#
# This script assumes the following release files have already been copied to your home directory:
#
# hootenanny-$NEW_VERSION.tar.gz
# hootenanny-services-$NEW_VERSION.war
# hootenanny-translations-$NEW_VERSION.tar.gz (optional)
#
# VARS IN THIS SECTION MUST BE SET EACH TIME THE SCRIPT IS RUN.
# name of the version you're deploying; version format is X.Y.Z; this should match the version of
# the archive files being deployed
NEW_VERSION=
# the user group used when installing hoot core; needed for compiling hoot core only; The user 
# executing this script must be a member of this group, and the group must have the correct 
# privileges to install files to HOOT_CORE_INSTALL_DIR.
CORE_INSTALL_USER_GROUP=
# CORE_RUNTIME_USER and CORE_RUNTIME_USER_GROUP are the group permissions used for hoot core at 
# runtime, which may diffes from what was used at install time.  For most installers running the web
# services, CORE_RUNTIME_USER=tomcat and CORE_RUNTIME_USER_GROUP=tomcat
CORE_RUNTIME_USER=
CORE_RUNTIME_USER_GROUP=
# usually: /usr/local
HOOT_CORE_INSTALL_DIR=

# used for storing the archives to be installed, backups, and temporary file storage
TEMP_DIR=~
TOMCAT_HOME=/var/lib/tomcat6/webapps
DEEGREE_DIR_ROOT=/usr/share/tomcat6
# relative to home dir; leave blank for no backups
BACKUP_DIR_NAME=Backup
DATE=`date +%m-%d-%Y`
# Add port 8080 to this URL if port forwarding to port 80 hasn't been activated.
BASE_SERVICES_URL="http://localhost"

# In general you should always update all parts of Hootenanny at the same time, but the variables 
# in the next section allow you to not have to do so.  Having this capability can be useful when 
# debugging install issues.

# clearing the database can be necessary b/c liquibase dropAll can take an extreme amount of time 
# to drop larger datasets (maybe not true with new database design?...); be SUPER CERTAIN about 
# enabling this one; won't run unless UPDATE_DATABASE is also enabled
CLEAR_DATABASE=false 
UPDATE_DATABASE=true
UPDATE_CORE=true
# Use this for adding optional functionality to hoot core at compile time; e.g. "--with-rnd"
CORE_CONFIGURATION_OPTIONS="--with-rnd"
UPDATE_FOUO_TRANSLATIONS=true
UPDATE_SERVICES=true
UPDATE_UI=true

if [ "$JAVA_HOME" == "" ]; then
  if [ "$UPDATE_SERVICES" == "true" ]; then 
    echo "Please populate JAVA_HOME."
    echo "Its usually located at something like: /usr/lib/jvm/java-1.7.0-openjdk-1.7.0.65.x86_64"
    exit -1
  fi
fi
if [ "$NEW_VERSION" == "" ]; then
  echo "Please enter a NEW_VERSION.  See script comments for details."
  exit -1
fi
if [ "$CORE_INSTALL_USER_GROUP" == "" ]; then
  echo "Please enter a CORE_INSTALL_USER_GROUP.  See script comments for details."
  exit -1
fi
if [ "$CORE_RUNTIME_USER" == "" ]; then
  echo "Please enter a CORE_RUNTIME_USER.  See script comments for details."
  exit -1
fi
if [ "$CORE_RUNTIME_USER_GROUP" == "" ]; then
  echo "Please enter a CORE_RUNTIME_USER_GROUP.  See script comments for details."
  exit -1
fi
if [ "$HOOT_CORE_INSTALL_DIR" == "" ]; then
  echo "Please enter a HOOT_CORE_INSTALL_DIR.  See script comments for details."
  exit -1
fi

cd $TEMP_DIR
mkdir -p $TEMP_DIR/$BACKUP_DIR_NAME

if [ "$UPDATE_DATABASE" == "true" ]; then
  if [ "$CLEAR_DATABASE" == "true" ]; then
    echo "Clear the database?  Are you ABSOLUTELY SURE? y/n?"
    read clear
    if [ "$clear" == "y" ]; then
      echo "Clearing the hoot database..."
      export HOOT_HOME=hootenanny-$NEW_VERSION
      cp hootenanny-$NEW_VERSION/conf/DatabaseConfig.sh.orig hootenanny-$NEW_VERSION/conf/DatabaseConfig.sh
      hootenanny-$NEW_VERSION/scripts/DeleteAllTables.sh
      export HOOT_HOME=
      echo "Database cleared."
    else
      echo "*NOT* clearing the hoot database..."
    fi
  fi
  echo
  echo "Updating the hoot database..."
  rm -rf hootenanny-services-temp
  mkdir hootenanny-services-temp
  cp hootenanny-services-$NEW_VERSION.war hootenanny-services-temp
  cd hootenanny-services-temp
  unzip -q hootenanny-services-$NEW_VERSION.war
  cd ..
  mv hootenanny-services-temp hootenanny-services-$NEW_VERSION
  # liquibase bases its decision on whether to apply an update partially on what
  # directory each changeset XML file resides in.  Therefore, you must execute
  # this command exactly as written, from the exact directory shown here.  If not,
  # unnecessary updates will be applied, which will result in SQL errors.
  cd hootenanny-services-$NEW_VERSION/WEB-INF
  liquibase --contexts=default,production --changeLogFile=classes/db/db.changelog-master.xml --promptForNonLocalDatabase=false --defaultsFile=classes/db/liquibase.properties --logLevel=warning --classpath=lib/postgresql-9.1-901-1.jdbc4.jar update
  cd $TEMP_DIR
  rm -rf hootenanny-services-$NEW_VERSION
  echo "hoot database updated."
  echo
  sleep 3
fi

if [ "$UPDATE_CORE" == "true" ]; then
  echo "Updating the hoot core..."
  if [ -d "$HOOT_CORE_INSTALL_DIR/customscript" ]; then
    # back up the custom translations; assumes install dir for this installation was the same as
    # the previous installation
    sudo cp -R $HOOT_CORE_INSTALL_DIR/customscript $TEMP_DIR
  fi
	# This directory could be left over from a previous failed install.
	if [ -d "$HOOT_CORE_INSTALL_DIR/hootenanny-$NEW_VERSION" ]; then
  	sudo rm -rf $HOOT_CORE_INSTALL_DIR/hootenanny-$NEW_VERSION
	fi
	# No need to backup hoot core, since we leave old versions copied and just update the symbolic 
  # link for new versions.
  sudo cp -R hootenanny-$NEW_VERSION $HOOT_CORE_INSTALL_DIR
  if [ -d "$HOOT_CORE_INSTALL_DIR/hoot" ]; then
    cd $HOOT_CORE_INSTALL_DIR/hoot
    sudo make uninstall
    cd ..
    sudo rm -f $HOOT_CORE_INSTALL_DIR/hoot 
    sudo rm -f $HOOT_CORE_INSTALL_DIR/bin/hoot
  fi
  sudo chown -R $USER:$CORE_INSTALL_USER_GROUP hootenanny-$NEW_VERSION
  cd hootenanny-$NEW_VERSION
  #TODO: remove this backup copy line after asciidoc is installed onsite
  sudo cp -R docs $TEMP_DIR
  source ./SetupEnv.sh
  touch conf/DatabaseConfig.sh
  ./configure -q --prefix=$HOOT_CORE_INSTALL_DIR/hootenanny-$NEW_VERSION $CORE_CONFIGURATION_OPTIONS && make -s clean-all && make -sj8 && sudo make -s install
  #TODO: replace the docs copy command here with these two commented out lines after asciidoc is installed onsite
  sudo cp -R $TEMP_DIR/docs .
  #cd docs
  #make
  if [ -d "$HOOT_CORE_INSTALL_DIR/customscript" ]; then
    # restore the custom translations
    sudo cp -R $TEMP_DIR/customscript .
  fi
  cd $HOOT_CORE_INSTALL_DIR
  sudo ln -s $HOOT_CORE_INSTALL_DIR/hootenanny-$NEW_VERSION $HOOT_CORE_INSTALL_DIR/hoot
  sudo chmod -R 755 $HOOT_CORE_INSTALL_DIR/hoot
  sudo chown -R $CORE_RUNTIME_USER:$CORE_RUNTIME_USER_GROUP $HOOT_CORE_INSTALL_DIR/hoot
  sudo chmod -R 755 hootenanny-$NEW_VERSION
  sudo chown -R $CORE_RUNTIME_USER:$CORE_RUNTIME_USER_GROUP hootenanny-$NEW_VERSION
  sudo ln -s $HOOT_CORE_INSTALL_DIR/hoot/bin/hoot $HOOT_CORE_INSTALL_DIR/bin/hoot
  hoot --version
  echo "hoot core updated."
  echo
  sleep 3
fi

if [ "$UPDATE_FOUO_TRANSLATIONS" == "true" ]; then
  echo "Updating the FOUO translations..."
  cd $TEMP_DIR
  rm -rf plugins-local
  rm -rf translations-local
  tar -xvzf hootenanny-translations-$NEW_VERSION.tar.gz > /dev/null
	# No need to backup these translations, since they are kept under the hoot root directory
	# with previous versions.
  sudo cp -R plugins-local $HOOT_CORE_INSTALL_DIR/hoot
	rm -rf plugins-local
  sudo chown -R $CORE_RUNTIME_USER:$CORE_RUNTIME_USER_GROUP $HOOT_CORE_INSTALL_DIR/hoot/plugins-local
  sudo chmod -R 755 $HOOT_CORE_INSTALL_DIR/hoot/plugins-local
  sudo cp -R translations-local $HOOT_CORE_INSTALL_DIR/hoot
	rm -rf translations-local
  sudo chown -R $CORE_RUNTIME_USER:$CORE_RUNTIME_USER_GROUP $HOOT_CORE_INSTALL_DIR/hoot/translations-local
  sudo chmod -R 755 $HOOT_CORE_INSTALL_DIR/hoot/translations-local
  echo "FOUO translations updated."
  echo
  sleep 3
fi

if [ "$UPDATE_SERVICES" == "true" ]; then
  echo "Updating the hoot services..."
	sudo mkdir -p $DEEGREE_DIR_ROOT/.deegree
	sudo chown root:$CORE_RUNTIME_USER_GROUP $DEEGREE_DIR_ROOT/.deegree	
	cd $TOMCAT_HOME
	if [ "$BACKUP_DIR_NAME" != "" ]; then
    if [ -f "hoot-services.war" ]; then
  	  sudo mv hoot-services.war $TEMP_DIR/$BACKUP_DIR_NAME/hootenanny-services-$DATE.war
    fi
	else
    if [ -f "hoot-services.war" ]; then
		  rm -f hoot-services.war
    fi
	fi
  # just in case any other war files are in there...
  sudo rm -f hoot*-services*.war
  if [ -f webapps/hoot-services/WEB-INF/classes/conf/local.conf ]; then
    sudo cp webapps/hoot-services/WEB-INF/classes/conf/local.conf $TEMP_DIR
    SERVICES_LOCAL_CONF_FILE_EXISTS=true
    echo "Found existing services local conf file.  Backed up to $TEMP_DIR."
  else
    SERVICES_LOCAL_CONF_FILE_EXISTS=false
    echo "No existing services local.conf file found.  No backup of it needs to be made."
  fi
  if [ -d "hoot-services" ]; then
    sudo rm -rf hoot-services
  fi
  sudo chmod 754 $TEMP_DIR/hootenanny-services-$NEW_VERSION.war
  sudo chown root:$CORE_RUNTIME_USER_GROUP $TEMP_DIR/hootenanny-services-$NEW_VERSION.war
  sudo cp $TEMP_DIR/hootenanny-services-$NEW_VERSION.war hoot-services.war
  echo "Waiting for web server to extract war file initially..."
  sleep 10
  if [ "$SERVICES_LOCAL_CONF_FILE_EXISTS" == "true" ]; then
    sudo cp $TEMP_DIR/local.conf webapps/hoot-services/WEB-INF/classes/conf
    echo "Copied backed up version of services local.conf file."
  fi
  sudo chown -R root:$CORE_RUNTIME_USER_GROUP hoot-services
  sudo chmod -R 775 hoot-services
	# needed by WFS
  sudo chmod -R 777 hoot-services/WEB-INF/workspace
  sudo service tomcat6 restart
  echo "hoot services updated."
  echo
  sleep 3
fi

if [ "$UPDATE_UI" == "true" ]; then
  echo "Updating the hoot ui..."
	cd $TOMCAT_HOME
  # check to see if the imagery file already exists
  if [ -f hootenanny-id/data/imagery.json ]; then
    cp hootenanny-id/data/imagery.json $TEMP_DIR
    IMAGERY_FILE_EXISTS=true
    echo "Found existing imagery.json file and backed up to: " $TEMP_DIR
  else
    IMAGERY_FILE_EXISTS=false
    echo "No existing imagery.json file found.  Using default."
  fi
  # No need to first back up hoot iD, since the previous version will exist in the last hoot 
  # core install dir.
  sudo rm -rf hootenanny-id
  sudo cp -R $HOOT_CORE_INSTALL_DIR/hoot/hoot-ui/ hootenanny-id
  sudo chown -R root:$CORE_RUNTIME_USER_GROUP hootenanny-id
  sudo chmod -R +r hootenanny-id
  sudo find hootenanny-id -type d -exec chmod +x {} ';' # TODO: What does this do?
  if [ "$IMAGERY_FILE_EXISTS" == "true" ]; then
    echo "Restoring backed up custom basemaps..."
    cd hootenanny-id/data
    sudo cp $TEMP_DIR/imagery.json hootenanny-id/data
  fi
  echo "hoot ui updated."
  echo
  sleep 3
fi

echo "Hootenanny update complete to version $NEW_VERSION.  Access the command line interface by typing: hoot."
if [ "$UPDATE_UI" == "true" ]; then
  echo "View the web interface at " $BASE_SERVICES_URL"/hootenanny-id"
  echo "Verify the correct version numbers by clicking on the InnoVision logo."
else
  echo "Verify the correct version is installed with by typing: hoot --version."
fi
cd $TEMP_DIR
