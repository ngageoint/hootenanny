# Installing Vagrant

To download Vagrant for supported platforms, see [here](https://www.vagrantup.com/downloads.html)

# Installing VirtualBox

If choosing VirtualBox as your virtualization provider, see [here](https://www.virtualbox.org/wiki/Downloads).  

You may have to enable hardware virtualization extensions in your BIOS before using it.

If running on Ubuntu, you may have to install a newer version of VirtualBox than what is available in the public repositories in order for it to work correctly with Vagrant.

# Setting up Hootennany with Vagrant & VirtualBox

Once Vagrant has been installed, you can start an environment by checking out the hoot code, then changing to the directory which contains the Vagrantfile by typing:

    # Windows users will need to uncomment the line ending configuration option.
    git clone git@github.com:ngageoint/hootenanny.git hoot #--config core.autocrlf=input
    cd hoot
    git submodule init
    git submodule update
    vagrant up

# Other Virtualization Providers

If you would like to use Parallels instead of VirtualBox, please run the following command:
```
vagrant up --provider=parallels
```
Please note that this requires the Parallels Vagrant plugin, which can be installed:
```
vagrant plugin install vagrant-parallels
```

Similarly, if you would like to use VMware Workstation instead of VirtualBox, please run the following command:
```
vagrant up --provider vmware_workstation
```
Please note that this requires the VMware Vagrant plugin, which can be installed:
```
vagrant plugin install vagrant-vmware-workstation
```

# Vagrant Provisioning

The initialization of the vagrant vm will take about an hour to download required software from the internet and set it up as a running system. ~~Once it is complete, uncomment the `#, group: "tomcat6"` portion of the in Vagrantfile to allow the webapp to write to shared folders.~~  The previous workaround step is no longer needed as the provision script adds vagrant and tomcat6 users to each others group.

You should be able to log into the running VM by typing:

    vagrant ssh

Within this login shell, you can build the code, run the server or the tests. For example, to run the tests:

    vagrant ssh
    cd hoot
    source ./SetupEnv.sh
    scripts/CopyWebAppsToTomcat.sh
    make -sj$(nproc) test-all

# Using Hootenanny

To access the web pages you access the site in your [local Chrome browser](http://localhost:8888/hootenanny-id).

To run hoot from commandline

    vagrant ssh
    cd hoot
    source ./SetupEnv.sh
    hoot help

If you've updated the code, you must connect to the vm via ssh to build and redeploy to Tomcat:

    vagrant ssh
    cd hoot
    source ./SetupEnv.sh
    make -sj$(nproc)
    sudo -u tomcat6 scripts/vagrantDeployTomcat.sh 

