# Prerequisites

[Vagrant](https://www.vagrantup.com/downloads.html)

[VirtualBox](https://www.virtualbox.org/wiki/Downloads)

[git](https://git-scm.com/)

In some cases, you may have to install a newer version of Vagrant or VirtualBox than what is available in the public repositories for your host operating system in order to launch the virtual machine.

You will also need some Vagrant plugins:

    vagrant plugin install vagrant-bindfs

You may also need to enable hardware virtualization extensions in your BIOS.

# Setting Up Hootenanny

Hootenanny is thoroughly tested against the Virtualbox provider.  You are welcome to try other providers, and they can be configured in Vagrantfile.  Make sure the the umask of the terminal used to start the vagrant vm is set to `002` (see [#1332](https://github.com/ngageoint/hootenanny/issues/1382))

Once the prerequisites have been installed, you can set up the Hootenanny environment.

    git clone git@github.com:ngageoint/hootenanny.git hoot
    cd hoot
    git submodule init
    git submodule update
    vagrant up
    
Please note, cloning using SSH is supported only with an authenticated GitHub account. To use anonymous cloning, replace the first line with SSL cloning like this:
    
    git clone https://github.com/ngageoint/hootenanny.git hoot --config core.autocrlf=input

~~Once initialization is complete, uncomment the `#, group: "tomcat8"` portion of the in Vagrantfile to allow the webapp to write to shared folders.~~  The previous workaround step is no longer needed as the provision script adds vagrant and tomcat8 users to each others group.

Log into the Hootenanny virtual machine:

    vagrant ssh

# Using Hootenanny

To access the Hoot2x iD Editor web user interface: [local Chrome browser](http://localhost:8888/hootenanny-id/).

To run Hootenanny from the command line:

    vagrant ssh
    cd hoot
    source ./SetupEnv.sh
    hoot help

# Running Hootenanny Tests

To run the Hootenanny tests:

    vagrant ssh
    cd hoot
    source ./SetupEnv.sh
    make -sj$(nproc) test-all

# Modifying Hootenanny Code

If you've updated the Hootenanny code, perform the following steps to redeploy it:

    vagrant ssh
    cd hoot
    source ./SetupEnv.sh
    make -sj$(nproc)
    ~~sudo -u tomcat8~~ scripts/tomcat/CopyWebAppsToTomcat.sh

If you typically use hoot-server for development, make sure that the UI on localhost:8080 matches localhost:8888. See [here](https://github.com/ngageoint/hootenanny/blob/develop/test-files/ui/README.md) for further instructions on how to run tests.

If you run into permission errors running the Tomcat deployment script, remove files that may be owned by Tomcat and then re-run the script:

    sudo rm -rf /usr/share/tomcat8/webapps/hootenannyid
    sudo rm -rf /usr/share/tomcat8/webapps/hoot-services.war
    sudo rm -rf /usr/share/tomcat8/webapps/hoot-services
    scripts/tomcat/CopyWebAppsToTomcat.sh
