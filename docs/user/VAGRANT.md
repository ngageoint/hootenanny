# Prerequisites

[Vagrant](https://www.vagrantup.com/downloads.html)

[VirtualBox](https://www.virtualbox.org/wiki/Downloads)

[git](https://git-scm.com/)

In some cases, you may have to install a newer version of Vagrant or VirtualBox than what is available in the public repositories for your host operating system in order to launch the virtual machine.

You will also need some Vagrant plugins:

    vagrant plugin install vagrant-bindfs vagrant-sshfs

You may also need to enable hardware virtualization extensions in your BIOS.

# Setting Up Hootenanny

Hootenanny is thoroughly tested against the Virtualbox provider.  You are welcome to try other providers, and they can be configured in Vagrantfile.  Make sure the the umask of the terminal used to start the vagrant vm is set to `002` (see [#1332](https://github.com/ngageoint/hootenanny/issues/1382))

Once the prerequisites have been installed, you can set up the Hootenanny environment:

    # Windows users will need to uncomment the line ending configuration option.
    git clone git@github.com:ngageoint/hootenanny.git hoot #--config core.autocrlf=input
    cd hoot
    git submodule init
    git submodule update
    vagrant up

~~Once initialization is complete, uncomment the `#, group: "tomcat8"` portion of the in Vagrantfile to allow the webapp to write to shared folders.~~  The previous workaround step is no longer needed as the provision script adds vagrant and tomcat8 users to each others group.

Log into the Hootenanny virtual machine:

    vagrant ssh
    
# Known issues

A) If you get error like this, it might be that you need to install VirtualBox Guest Addittions manually.

```
Vagrant was unable to mount VirtualBox shared folders. <...>

mount -t vboxsf -o uid=1000,gid=1000 home_vagrant_hoot /home/vagrant/hoot

The error output from the command was:

/sbin/mount.vboxsf: mounting failed with the error: Invalid argument

```
Follow these steps:
1. Login into VM
     
    vagrant ssh
    
2. Run setup of VBoxGuestAdditions like this:

    cd /opt/VBoxGuestAdditions-*/init  
    sudo ./vboxadd setup
    sudo /sbin/rcvboxadd quicksetup all
    sudo reboot
    
3. Try again.
    
    vagrant up --provision
    
You might need to check errors and/or try command multiple times. 

B) If you see build to freeze at this point, be patient ` default: hoot-services/src/main/resources/language-translation/langdetect-183.bin: OK`
It may take an hour and more to finish this step, even on a decent PC.

# Windows hints

It is possible to install Hootenanny on Windows using Vagrant. Alternatively, you can use WSL2 + [CentOS7 image](https://github.com/mishamosher/CentOS-WSL) and [RPM install](https://github.com/ngageoint/hootenanny-rpms/blob/master/docs/install.md), which is somewhat easier, faster and light on PC requirements. However, at the moment, RPM version is w/o JOSM validation integrations.

To install using Vagrant, first, make sure you have tools above. If you don't, then, if you have [Chocolatey](https://chocolatey.org/install), run in the elevated shell:

    cinst git vagrant virtualbox 
    
Tt may be beneficial to disable Windows Defender (temporarily), otherwise build can fail with message "unable to ruby install some Ruby gem".
Make sure you are using PowerShell and not PSCore to run Vagrant, otherwise it will complain that Powershell cannot be found.
  
   
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
