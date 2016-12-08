#!/usr/bin/env bash

TOMCAT_NAME=tomcat8
TOMCAT_GROUP=tomcat8
TOMCAT_USER=tomcat8
TOMCAT_HOME=/usr/share/tomcat8
TOMCAT_USER_HOME=/var/lib/tomcat8
TOMCAT_CACHE_HOME=/var/cache/tomcat8
TOMCAT_LOGS_HOME=/var/log/tomcat8
SYSCONFDIR=/etc


# Uninstall tomcat6 if present
#if dpkg -l | grep --quiet tomcat6; then
#    apt-get -y purge tomcat6
#    apt-get -y autoremove
#    rm -f /etc/default/tomcat6.bak
#fi

#ret=false
#getent passwd tomcat6 >/dev/null 2>&1 && ret=true

#if $ret; then
#    deluser tomcat6
#fi

# Delete tomcat6 group if exists
#if grep --quiet "^tomcat6:" /etc/group; then
#    groupdel tomcat6
#fi

# Remove Tomcat6 traces
#if grep --quiet TOMCAT6_HOME ~/.profile; then
#    echo "### Removing TOMCAT6_HOME from profile..."
#    sed -i '/TOMCAT6_HOME/d' ~/.profile
#fi

#if [ -f /etc/default/tomcat6 ] ; then
#    rm -f /etc/default/tomcat6
#fi

# Shut down Tomcat 6 service
service tomcat6 stop

# Prevent tomcat6 from being started during boot time
update-rc.d -f tomcat6 remove

echo "### Installing Tomcat 8.5.8"

# Create tomcat8 group if not already created
getent group $TOMCAT_GROUP >/dev/null || groupadd -r $TOMCAT_GROUP

# Create tomcat8 user if not already created
getent passwd $TOMCAT_USER >/dev/null || useradd --comment "Tomcat 8 Daemon User" --shell /bin/bash -M -r -g $TOMCAT_GROUP --home $TOMCAT_HOME $TOMCAT_USER

# Add tomcat8 and vagrant to each others groups so we can get the group write working with nfs
if ! groups vagrant | grep --quiet "\b$TOMCAT_GROUP\b"; then
    echo "Adding vagrant user to $TOMCAT_GROUP user group..."
    usermod -a -G $TOMCAT_GROUP vagrant
fi

# Add tomcat8 and vagrant to each others groups so we can get the group write working with nfs
if ! groups $TOMCAT_GROUP | grep --quiet "\bvagrant\b"; then
    echo "Adding $TOMCAT_GROUP user to vagrant user group..."
    usermod -a -G vagrant $TOMCAT_GROUP
fi

wget --quiet https://archive.apache.org/dist/tomcat/tomcat-8/v8.5.8/bin/apache-tomcat-8.5.8.tar.gz -P /tmp
mkdir $TOMCAT_HOME
tar -zxf /tmp/apache-tomcat-8.5.8.tar.gz -C $TOMCAT_HOME --strip-components 1
chmod -R 755 $TOMCAT_HOME

rm -rf $TOMCAT_HOME/webapps
mkdir -p $TOMCAT_USER_HOME/webapps
ln -s $TOMCAT_USER_HOME/webapps webapps
chmod 775 $TOMCAT_USER_HOME

#We don't need .bat file lying around
rm -f $TOMCAT_HOME/bin/*.bat

# Put logging in /var/log and link back.
rm -rf $TOMCAT_HOME/logs
mkdir -p $TOMCAT_LOGS_HOME
chmod -R 755 $TOMCAT_LOGS_HOME
cd $TOMCAT_HOME
ln -s $TOMCAT_LOGS_HOME logs

cd -

# Put conf in /etc/ and link back.
mv $TOMCAT_HOME/conf $SYSCONFDIR/$TOMCAT_NAME
cd $TOMCAT_HOME
ln -s $SYSCONFDIR/$TOMCAT_NAME conf
cd -

# Put temp and work to /var/cache and link back.
mkdir -p $TOMCAT_CACHE_HOME

mkdir $TOMCAT_HOME/.deegree
chown $TOMCAT_GROUP:$TOMCAT_USER $TOMCAT_HOME/.deegree

mv $TOMCAT_HOME/temp $TOMCAT_CACHE_HOME
mv $TOMCAT_HOME/work $TOMCAT_CACHE_HOME

cd $TOMCAT_HOME

ln -s $TOMCAT_CACHE_HOME/temp
ln -s $TOMCAT_CACHE_HOME/work
chmod 775 $TOMCAT_CACHE_HOME/temp
chmod 775 $TOMCAT_CACHE_HOME/work

cd -

# Trying this to try to get rid of errors
mkdir -p $TOMCAT_HOME/server/classes
mkdir -p $TOMCAT_HOME/shared/classes
chown -R $TOMCAT_GROUP:$TOMCAT_USER $TOMCAT_HOME/server
chown -R $TOMCAT_GROUP:$TOMCAT_USER $TOMCAT_HOME/shared


# Drop sbin script
install -d -m 755 %{buildroot}/%{_sbindir}
install    -m 755 %_sourcedir/%{name}.bin %{buildroot}/%{_sbindir}/%{name}

# Drop init script
install -d -m 755 %{buildroot}/%{_initrddir}
install    -m 755 %_sourcedir/%{name}.init %{buildroot}/%{_initrddir}/%{name}

# Drop sysconfig script
install -d -m 755 %{buildroot}/%{_sysconfdir}/sysconfig/
install    -m 644 %_sourcedir/%{name}.sysconfig %{buildroot}/%{_sysconfdir}/sysconfig/%{name}

# Drop conf script
install    -m 644 %_sourcedir/%{name}.conf %{buildroot}/%{_sysconfdir}/%{name}

# Drop logrotate script
install -d -m 755 %{buildroot}/%{_sysconfdir}/logrotate.d
install    -m 644 %_sourcedir/%{name}.logrotate %{buildroot}/%{_sysconfdir}/logrotate.d/%{name}

#chkconfig --add %{name}


# Configure Tomcat
if ! grep --quiet TOMCAT8_HOME ~/.profile; then
    echo "### Adding Tomcat to profile..."
    echo "export TOMCAT8_HOME=$TOMCAT_HOME" >> ~/.profile
    source ~/.profile
fi

if grep -i --quiet 'gdal/1.10' $SYSCONFDIR/default/$TOMCAT_NAME; then
    echo "### Fixing Tomcat GDAL_DATA env var path..."
    sed -i.bak s@^GDAL_DATA=.*@GDAL_DATA=\/usr\/local\/share\/gdal@ $SYSCONFDIR/default/tomcat8
fi

if ! grep -i --quiet 'ingest/processed' $SYSCONFDIR/$TOMCAT_NAME/server.xml; then
    echo "Adding Tomcat context path for tile images..."
    sudo sed -i.bak 's@<\/Host>@  <Context docBase=\"'"$HOOT_HOME"'\/ingest\/processed\" path=\"\/static\" \/>\n      &@' $SYSCONFDIR/$TOMCAT_NAME/server.xml
fi

if ! grep -i --quiet 'allowLinking="true"' $SYSCONFDIR/$TOMCAT_NAME/context.xml; then
    echo "Set allowLinking to true in Tomcat context..."
    sudo sed -i.bak "s@^<Context>@<Context allowLinking=\"true\">@" $SYSCONFDIR/$TOMCAT_NAME/context.xml
fi

# Clean out tomcat logfile. We restart tomcat after provisioning
service $TOMCAT_NAME stop
rm $TOMCAT_LOGS_HOME/catalina.out

