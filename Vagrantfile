# -*- mode: ruby -*-
# vi: set ft=ruby :

# All Vagrant configuration is done below. The "2" in Vagrant.configure
# configures the configuration version (we support older styles for
# backwards compatibility). Please don't change it unless you know what
# you're doing.
Vagrant.configure(2) do |config|
  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine. In the example below,
  # accessing "localhost:8080" will access port 80 on the guest machine.

  tomcatPort = ENV['TOMCAT_PORT']
  if tomcatPort.nil?
    tomcatPort = '9888'
  end

  transPort = ENV['NODEJS_PORT']
  if transPort.nil?
    transPort = '9894'
  end

  mergePort = ENV['P2P_PORT']
  if mergePort.nil?
    mergePort = '9896'
  end

  mapnikPort = ENV['NODE_MAPNIK_SERVER_PORT']
  if mapnikPort.nil?
    mapnikPort = '9800'
  end

  # tomcat service
  config.vm.network "forwarded_port", guest: 8080, host: tomcatPort
  # translation nodejs service
  config.vm.network "forwarded_port", guest: 8094, host: transPort
  # merge nodejs service
  config.vm.network "forwarded_port", guest: 8096, host: mergePort
  # node-mapnik-server nodejs service
  config.vm.network "forwarded_port", guest: 8000, host: mapnikPort

  # Global settings - default for Ubuntu1404


  # Ubuntu1404 Box
  # This is the standard, working box
  config.vm.define "hoot", primary: true do |hoot|
    hoot.vm.box = "ubuntu/trusty64"
    hoot.vm.box_url = "https://atlas.hashicorp.com/ubuntu/boxes/trusty64"
    hoot.vm.synced_folder ".", "/home/vagrant/hoot"
    hoot.vm.provision "hoot", type: "shell", :privileged => false, :path => "VagrantProvision.sh"
#      config.vm.provision "build", type: "shell", :privileged => false, :path => "VagrantBuild.sh"
#      config.vm.provision "tomcat", type: "shell", :privileged => false, :inline => "sudo service tomcat8 restart", run: "always"
#      config.vm.provision "mapnik", type: "shell", :privileged => false, :inline => "sudo service node-mapnik-server start", run: "always"
#      config.vm.provision "hadoop", type: "shell", :privileged => false, :inline => "stop-all.sh && start-all.sh", run: "always"
  end

  # Ubuntu1604 Box
  # For testing before we upgrade from Ubuntu1404
  config.vm.define "hoot_ubuntu1604", autostart: false do |hoot_ubuntu1604|
    hoot_ubuntu1604.vm.box = "ubuntu/xenial64"
    hoot_ubuntu1604.vm.box_url = "https://atlas.hashicorp.com/ubuntu/boxes/xenial64"

    # Why does this box have an ubuntu user, not a vagrant user?   Why?????
    config.vm.synced_folder ".", "/home/ubuntu/hoot"

    hoot_ubuntu1604.vm.provision "hoot", type: "shell", :privileged => false, :path => "VagrantProvisionUbuntu1604.sh"
    hoot_ubuntu1604.vm.provision "build", type: "shell", :privileged => false, :path => "VagrantBuild.sh"
    #hoot_ubuntu1604.vm.provision "tomcat", type: "shell", :privileged => false, :inline => "sudo service tomcat8 restart", run: "always"
    #hoot_ubuntu1604.vm.provision "mapnik", type: "shell", :privileged => false, :inline => "sudo service node-mapnik-server start", run: "always"
    #hoot_ubuntu1604.vm.provision "hadoop", type: "shell", :privileged => false, :inline => "stop-all.sh && start-all.sh", run: "always"
  end


  # Centos7 box
  # For testing
  config.vm.define "hoot_centos7", autostart: false do |hoot_centos7|
    # This seems to be the "latest" version of centos7
    # hoot_centos7.vm.box = "centos/7"
    # hoot_centos7.vm.box_url = "https://atlas.hashicorp.com/centos/boxes/7"

    # There are a lot of different Centos7.2 boxes.
    hoot_centos7.vm.box = "bento/centos-7.2"
    hoot_centos7.vm.box_url = "https://atlas.hashicorp.com/bento/boxes/centos-7.2"

    # Stop the default vagrant rsyncing
    config.vm.synced_folder '.', '/home/vagrant/sync', disabled: true

    # NOTE: Networking needs some tweaking.
    # If you don't want to use NFS, then something like this will work:
    # hoot_centos7.vm.synced_folder '.', '/home/vagrant/hoot', type: 'rsync'

    hoot_centos7.vm.network "private_network", ip: "192.168.33.10"
    hoot_centos7.nfs.map_uid = Process.uid
    hoot_centos7.nfs.map_gid = Process.gid
    # Enabled async, this makes small file writes much faster (e.g. unzip)
    hoot_centos7.vm.synced_folder ".", "/home/vagrant/.hoot-nfs", type: "nfs", :linux__nfs_options => ['rw','no_subtree_check','all_squash','async']
    hoot_centos7.bindfs.bind_folder "/home/vagrant/.hoot-nfs", "/home/vagrant/hoot", perms: nil

    hoot_centos7.vm.provision "hoot", type: "shell", :privileged => false, :path => "VagrantProvisionCentOS7.sh"
    #hoot_centos7.vm.provision "build", type: "shell", :privileged => false, :path => "VagrantBuild.sh"
    #hoot_centos7.vm.provision "tomcat", type: "shell", :privileged => false, :inline => "sudo service tomcat8 restart", run: "always"
    #hoot_centos7.vm.provision "mapnik", type: "shell", :privileged => false, :inline => "sudo service node-mapnik-server start", run: "always"
  end

  # Provider-specific configuration so you can fine-tune various
  # backing providers for Vagrant. These expose provider-specific options.
  # Example for VirtualBox:
  #
  config.vm.provider "virtualbox" do |vb|
  #   # Display the VirtualBox GUI when booting the machine
  #   vb.gui = true

  #   # Customize the amount of memory on the VM:
     vb.memory = 8192
     vb.cpus = 4
  end

  # This is a provider for the Parallels Virtualization Software
  # Run "vagrant up --provider=parallels" to spin up using parallels.
  config.vm.provider "parallels" do |para, override|
        para.memory = 8192
        para.cpus = 4
        override.hoot.vm.box = "parallels/ubuntu-14.04"
        override.hoot.vm.box_url = "https://atlas.hashicorp.com/parallels/boxes/ubuntu-14.04"

        # NOTE: I have no way to test these - mattj
        override.hoot_ubuntu1604.vm.box = "parallels/ubuntu-16.04"
        override.hoot_ubuntu1604.vm.box_url = "https://atlas.hashicorp.com/parallels/boxes/ubuntu-16.04"
        override.hoot_centos7.vm.box = "parallels/centos-7.3"
        override.hoot_centos7.vm.box_url = "https://atlas.hashicorp.com/parallels/boxes/centos-7.3"
  end

  # This is a provider for VMware Workstation
  # Run "vagrant up --provider=vmware_workstation" to spin up using VMware.
  config.vm.provider "vmware_workstation" do |vw, override|
      vw.memory = 8192
      vw.cpus = 4
      override.hoot.vm.box = "puphpet/ubuntu1404-x64"
      override.hoot.vm.box_url = "https://atlas.hashicorp.com/puphpet/boxes/ubuntu1404-x64"

      # NOTE: Yet again, I have no way to test this - mattj
      override.hoot_ubuntu1604.vm.box = "puphpet/ubuntu1604-x64"
      override.hoot_ubuntu1604.vm.box_url = "https://atlas.hashicorp.com/puphpet/boxes/ubuntu1604-x64"
  end

end

# Allow local overrides of vagrant settings
if File.exists?('VagrantfileLocal')
  load 'VagrantfileLocal'
else
  if File.exists?('VagrantfileLocal.vbox')
    load 'VagrantfileLocal.vbox'
  end
end


