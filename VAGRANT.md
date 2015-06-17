# Installing Vagrant

To download Vagrant for supported platforms, see [here](http://www.vagrantup.com/downloads).

# Setting up Hootennany

Once Vagrant has been installed, you can start an environment by checking out the hoot code, then changing to the directory which contains the Vagrantfile by typing:

    git clone git@github.com:ngageoint/hootenanny.git hoot
    cd hoot
    git submodule init
    git submodule update
    vagrant up


This will take a up to two hours to download required software from the internet and set it up as a running system. Once it is complete, uncomment the '#, group: "tomcat6"' portion of the in Vagrantfile to allow the webapp to write to shared folders.

    # argument is a set of non-required options.
    config.vm.synced_folder ".", "/home/vagrant/hoot", group: "tomcat6"
    # UNCOMMENT group after inital provisioning, then run vagrant reload

Then issue the reload command for the changes to take effect on the vm:

    vagrant reload

You should be able to log into the running VM by typing:

    vagrant ssh

Within this login shell, you can build the code, run the server or the tests. For example, to run the tests:

    cd hoot
    source ./SetupEnv.sh
    make test

To access the web pages you access the site in your [local browser](http://localhost:8888/hootenanny-id).

If you've updated the code, you must connect to the vm via ssh to build and redeploy to Tomcat:

    vagrant ssh
    cd hoot
    source ./SetupEnv.sh
    scripts/ezClean.sh
    scripts/ezBuildAll.sh
    sudo -u tomcat6 scripts/vagrantDeployTomcat.sh
