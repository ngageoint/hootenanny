FROM centos:7

ARG HOOT_USER=hoot
ENV HOOT_HOME=/home/$HOOT_USER/hootenanny
ARG HOOT_REPO=https://github.com/ngageoint/hootenanny

RUN yum -y install git \
    which \
    mlocate \
    sudo

# Add a user with sudo access
RUN mkdir -p /etc/sudoers.d
RUN mkdir -p $HOOT_HOME

RUN useradd -mUl -G wheel -s /bin/bash $HOOT_USER -d /home/$HOOT_USER \
 && echo "$HOOT_USER ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/$HOOT_USER \
 && chmod 0440 /etc/sudoers.d/$HOOT_USER

RUN chown -R $HOOT_USER:$HOOT_USER /home/$HOOT_USER

USER $HOOT_USER
ARG SUDO=/usr/bin/sudo

WORKDIR $HOOT_HOME

RUN git clone $HOOT_REPO .
RUN git submodule init && git submodule update


# Common set of file versions
ENV JDK_VERSION=1.8.0_161
ENV JDK_URL=http://download.oracle.com/otn-pub/java/jdk/8u161-b12/2f38c3b165be4555a1fa6e98c45e0808/jdk-8u161-linux-x64.tar.gz
ENV JDK_TAR=jdk-8u161-linux-x64.tar.gz
ENV JDK_MD5=99051574a0d90871ed24a91a5d321ed2

# Hoot deps library versions
ENV GLPK_VERSION=4.64
ENV LIBOAUTHCPP_VERSION=0.1.0
ENV LIBPHONENUMBER_VERSION=8.12.27
ENV NODE_VERSION=14.16.1
ENV STXXL_VERSION=1.3.1
ENV GOOGLE_CHROME_VERSION=91.0.4472.114

# Geoint deps library versions
ENV ARMADILLO_VERSION=8.600.1
ENV GDAL_VERSION=3.2.3
ENV GEOS_VERSION=3.9.1
ENV LIBGEOTIFF_VERSION=1.6.0
ENV PROJ_VERSION=7.2.1

# PostgreSQL version
ENV POSTGRESQL_VERSION=13
ENV POSTGRESQL_VERSION_DOTLESS=${POSTGRESQL_VERSION}

# FGDB 1.5 is required to compile using g++ >= 5.1
# https://trac.osgeo.org/gdal/wiki/FileGDB#HowtodealwithGCC5.1C11ABIonLinux
ENV FGDB_VERSION=1.5.1
ENV FGDB_URL=https://github.com/Esri/file-geodatabase-api/raw/master/FileGDB_API_${FGDB_VERSION}/

# Devtoolset
ENV DEVTOOLSET_VERSION=8

RUN echo "Adding additional software repositories..."

# Ensure that CentOS Yum repository data is GPG-verified.
RUN echo "### Configuring CentOS to verify repository metadata ###" >> CentOS_upgrade.txt
RUN $SUDO yum-config-manager \
  --save \
  --setopt=base.repo_gpgcheck=1 \
  --setopt=extras.repo_gpgcheck=1 \
  --setopt=updates.repo_gpgcheck=1 &> /dev/null

# add EPEL repo for extra packages
RUN echo "### Add epel repo ###" >> CentOS_upgrade.txt
RUN $SUDO yum -y install epel-release >> CentOS_upgrade.txt 2>&1

# add GEOINT for spatial libraries and utilities.
RUN echo "### Add geoint-deps repo ###" >> CentOS_upgrade.txt
RUN $SUDO $HOOT_HOME/scripts/yum/geoint-repo.sh

# add Hoot repo for our pre-built dependencies.
RUN echo "### Add Hoot repo ###" >> CentOS_upgrade.txt
RUN $SUDO $HOOT_HOME/scripts/yum/hoot-repo.sh

## configure PGDG repository for PostgreSQL
RUN echo "### Add pgdg repo ###" >> CentOS_upgrade.txt
RUN $SUDO $HOOT_HOME/scripts/yum/pgdg-repo.sh $POSTGRESQL_VERSION

# configure the devtoolset repository
RUN echo "### Add devtoolset repo and postgresql ${POSTGRESQL_VERSION} libraries ###"
RUN $SUDO yum install -y \
     centos-release-scl postgresql${POSTGRESQL_VERSION}-libs >> CentOS_upgrade.txt 2>&1
RUN $SUDO yum-config-manager --enable rhel-server-rhscl-7-rpms

RUN echo "Updating OS..."
RUN echo "### Yum Upgrade ###" >> CentOS_upgrade.txt
RUN $SUDO yum -q -y upgrade >> CentOS_upgrade.txt 2>&1

# Install the versionlock plugin version first.
RUN $SUDO yum install -y yum-plugin-versionlock >> CentOS_upgrade.txt 2>&1

RUN echo "### Installing libraries with locked versions"
RUN $SUDO yum install -y \
     armadillo-$ARMADILLO_VERSION \
     geos-$GEOS_VERSION \
     geos-devel-$GEOS_VERSION \
     glpk-$GLPK_VERSION \
     glpk-devel-$GLPK_VERSION \
     gdal-$GDAL_VERSION \
     gdal-devel-$GDAL_VERSION \
     gdal-python-tools-$GDAL_VERSION \
     google-chrome-stable-$GOOGLE_CHROME_VERSION \
     libgeotiff-$LIBGEOTIFF_VERSION \
     libgeotiff-devel-$LIBGEOTIFF_VERSION \
     libphonenumber-$LIBPHONENUMBER_VERSION \
     libphonenumber-devel-$LIBPHONENUMBER_VERSION \
     liboauthcpp-$LIBOAUTHCPP_VERSION \
     liboauthcpp-devel-$LIBOAUTHCPP_VERSION \
     nodejs-$NODE_VERSION \
     nodejs-devel-$NODE_VERSION \
     proj-$PROJ_VERSION \
     proj-devel-$PROJ_VERSION \
     stxxl-$STXXL_VERSION \
     stxxl-devel-$STXXL_VERSION

RUN echo "### Locking versions of libraries"
RUN $SUDO yum versionlock add \
     armadillo-$ARMADILLO_VERSION \
     geos-$GEOS_VERSION \
     geos-devel-$GEOS_VERSION \
     glpk-$GLPK_VERSION \
     glpk-devel-$GLPK_VERSION \
     gdal-$GDAL_VERSION \
     gdal-devel-$GDAL_VERSION \
     gdal-python-tools-$GDAL_VERSION \
     google-chrome-stable-$GOOGLE_CHROME_VERSION \
     libgeotiff-$LIBGEOTIFF_VERSION \
     libgeotiff-devel-$LIBGEOTIFF_VERSION \
     libphonenumber-$LIBPHONENUMBER_VERSION \
     libphonenumber-devel-$LIBPHONENUMBER_VERSION \
     liboauthcpp-$LIBOAUTHCPP_VERSION \
     liboauthcpp-devel-$LIBOAUTHCPP_VERSION \
     nodejs-$NODE_VERSION \
     nodejs-devel-$NODE_VERSION \
     proj-$PROJ_VERSION \
     proj-devel-$PROJ_VERSION \
     stxxl-$STXXL_VERSION \
     stxxl-devel-$STXXL_VERSION

# install useful and needed packages for working with hootenanny
RUN echo "### Installing dependencies from repos..."
RUN $SUDO yum -y install \
    asciidoc \
    autoconf \
    autoconf-archive \
    automake \
    bison \
    boost-devel \
    bzip2 \
    ccache \
    cmake \
    cppunit-devel \
    dblatex \
    devtoolset-$DEVTOOLSET_VERSION \
    doxygen \
    gcc-c++ \
    git \
    git-core \
    gnuplot \
    lcov \
    libffi-devel \
    libicu-devel \
    libpng-devel \
    libtool \
    m4 \
    maven \
    mlocate \
    opencv \
    opencv-core \
    opencv-devel \
    opencv-python \
    osmosis \
    java-1.8.0-openjdk \
    perl-XML-LibXML \
    libpostal-data \
    libpostal-devel \
    parallel \
    postgresql${POSTGRESQL_VERSION_DOTLESS} \
    postgresql${POSTGRESQL_VERSION_DOTLESS}-contrib \
    postgresql${POSTGRESQL_VERSION_DOTLESS}-devel \
    postgresql${POSTGRESQL_VERSION_DOTLESS}-server \
    protobuf \
    protobuf-compiler \
    protobuf-devel \
    python \
    python-devel \
    python3  \
    python3-devel \
    python3-matplotlib \
    python3-pip  \
    python3-setuptools \
    ruby \
    ruby-devel \
    rubygem-bundler \
    qt5-qtbase \
    qt5-qtbase-devel \
    qt5-qtbase-postgresql \
    qt5-qtwebkit \
    qt5-qtwebkit-devel \
    readline-devel \
    redhat-lsb-core \
    sqlite-devel \
    swig \
    tex-fonts-hebrew \
    texlive \
    texlive-collection-fontsrecommended \
    texlive-collection-langcyrillic \
    unzip \
    vim \
    wamerican-insane \
    w3m \
    wget \
    words \
    xorg-x11-server-Xvfb \
    zip

RUN $SUDO alternatives --install /usr/bin/qmake qmake /usr/bin/qmake-qt5 500

ENV PATH=$PATH:$HOOT_HOME/bin
ENV JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk
ENV GDAL_DATA=/usr/share/gdal


# Install gems with bundler and strict versioning (see Gemfile)
RUN bundle config set --local path vendor/bundle
RUN bundle install

# Make sure that we are in ~ before trying to wget & install stuff
WORKDIR /home/$HOOT_USER/

# Install Google Chrome and ChromeDriver.
RUN $HOOT_HOME/scripts/chrome/driver-install.sh

# devtoolset sudo is badly broken
RUN $SUDO mv /opt/rh/devtoolset-$DEVTOOLSET_VERSION/root/usr/bin/sudo \
    /opt/rh/devtoolset-$DEVTOOLSET_VERSION/root/usr/bin/sudo.old

# Configure PostgreSQL
RUN echo "### Configuring PostgreSQL..."
RUN echo "Adding PostgreSQL $POSTGRESQL_VERSION path vars to profile..."
RUN echo "export PATH=\$PATH:/usr/pgsql-$POSTGRESQL_VERSION/bin" >> ~/.bashrc

ARG PG_HOME=/usr/pgsql-$POSTGRESQL_VERSION

ARG PGDATA=/var/lib/pgsql/$POSTGRESQL_VERSION/data

# configure Postgres settings
RUN echo "Configuring PostgreSQL..."
ENV PGSETUP_INITDB_OPTIONS="-E 'UTF-8' --lc-collate='en_US.UTF-8' --lc-ctype='en_US.UTF-8'"
RUN $SUDO -u postgres $PG_HOME/bin/initdb $PGDATA

ARG PG_HB_CONF=/var/lib/pgsql/$POSTGRESQL_VERSION/data/pg_hba.conf
RUN $SUDO -u postgres cp $PG_HB_CONF $PG_HB_CONF.orig
RUN $SUDO -u postgres sed -i "1ihost    all            hoot            127.0.0.1/32            md5" $PG_HB_CONF
RUN $SUDO -u postgres sed -i "1ihost    all            hoot            ::1/128                 md5" $PG_HB_CONF

ARG POSTGRES_CONF=/var/lib/pgsql/$POSTGRESQL_VERSION/data/postgresql.conf
RUN $SUDO -u postgres cp $POSTGRES_CONF $POSTGRES_CONF.orig
RUN $SUDO sed -i s/^max_connections/\#max_connections/ $POSTGRES_CONF
RUN $SUDO sed -i s/^shared_buffers/\#shared_buffers/ $POSTGRES_CONF

RUN echo "##--------------" | $SUDO -u postgres tee -a $POSTGRES_CONF
RUN echo "## Hoot Settings" | $SUDO -u postgres tee -a $POSTGRES_CONF
RUN echo "##--------------" | $SUDO -u postgres tee -a $POSTGRES_CONF
RUN echo "max_connections = 1000" | $SUDO -u postgres tee -a $POSTGRES_CONF
RUN echo "shared_buffers = 1024MB" | $SUDO -u postgres tee -a $POSTGRES_CONF
RUN echo "max_files_per_process = 1000" | $SUDO -u postgres tee -a $POSTGRES_CONF
RUN echo "work_mem = 16MB" | $SUDO -u postgres tee -a $POSTGRES_CONF
RUN echo "maintenance_work_mem = 256MB" | $SUDO -u postgres tee -a $POSTGRES_CONF
RUN echo "autovacuum = off" | $SUDO -u postgres tee -a $POSTGRES_CONF

RUN echo "$SUDO -u postgres $PG_HOME/bin/pg_ctl restart -D $PGDATA -s -o '-p 5432' -w -t 300" >> ~/restart-postgres.sh
RUN chmod +x ~/restart-postgres.sh

# Install Tomcat 8
RUN echo "### Installing Tomcat8..."
ARG TOMCAT_HOME=/usr/share/tomcat8
ARG TOMCAT_LOGS=/var/log/tomcat8
ARG TOMCAT_CONFIG=/etc/tomcat8

RUN echo "######## Begin ${TOMCAT_NAME} installation ########"
# Install Tomcat from our package in Hootenanny's dependency repo.
RUN $SUDO yum install -y tomcat8

# Modify the invoking user to be a part of the tomcat group, so it
# can write same directories as the tomcat service user.
RUN $SUDO usermod -a -G tomcat $HOOT_USER

ARG HOOT_CONF=${TOMCAT_CONFIG}/conf.d/hoot.conf
RUN echo "export GDAL_DATA=/usr/share/gdal" >> $HOOT_CONF
RUN echo "export HOOT_HOME=$HOOT_HOME" >> $HOOT_CONF
RUN echo "export HOOT_WORKING_NAME=hootenanny" >> $HOOT_CONF
RUN echo "export JAVA_HOME=$JAVA_HOME" >> $HOOT_CONF
RUN echo "export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib:$JAVA_HOME/jre/lib/amd64/server:${HOOT_HOME}/lib" >> $HOOT_CONF
RUN echo "export PATH=${PATH}" >> $HOOT_CONF

RUN echo "Adding Tomcat context path for tile images..."
RUN $SUDO sed -i.bak 's@<\/Host>@  <Context docBase=\"'"$HOOT_HOME"'\/userfiles\/ingest\/processed\" path=\"\/static\" \/>\n      &@' ${TOMCAT_CONFIG}/server.xml

# Always start with a clean $HOOT_HOME/userfiles/tmp
RUN rm -rf $HOOT_HOME/userfiles/tmp

# This is defensive!
# We do this so that Tomcat doesnt. If it does, it screws the permissions up
RUN mkdir -p $HOOT_HOME/userfiles/tmp


# Update bash config
RUN echo "source /opt/rh/devtoolset-$DEVTOOLSET_VERSION/enable" >> ~/.bashrc
RUN echo "alias cls='clear'" >> ~/.bashrc
RUN echo "alias la='ls -la'" >> ~/.bashrc
RUN echo "PS1='\w\$ '" >> ~/.bashrc
# RUN echo "export LD_LIBRARY_PATH=$GDAL_LIB_DIR:$JAVA_HOME/jre/lib/amd64/server:$HOOT_HOME/lib:$LD_LIBRARY_PATH" >> ~/.bashrc



RUN printf "\n\n*** PLEASE RUN /home/$HOOT_USER/restart-postgres.sh\n     AND $HOOT_HOME/scripts/database/SetupHootDb.sh" \
    "TO SETUP THE HOOT DATABASE! ***\n\n\n"


