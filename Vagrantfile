# -*- mode: ruby -*-
# vi: set ft=ruby :

# All Vagrant configuration is done below. The "2" in Vagrant.configure
# configures the configuration version (we support older styles for
# backwards compatibility). Please don't change it unless you know what
# you're doing.
Vagrant.configure(2) do |config|
  # The most common configuration options are documented and commented below.
  # For a complete reference, please see the online documentation at
  # https://docs.vagrantup.com.

  # Every Vagrant development environment requires a box. You can search for
  # boxes at https://atlas.hashicorp.com/search.
  config.vm.box = "ubuntu/trusty64"
  config.vm.box_url = "https://atlas.hashicorp.com/ubuntu/boxes/trusty64"

  # Disable automatic box update checking. If you disable this, then
  # boxes will only be checked for updates when the user runs
  # `vagrant box outdated`. This is not recommended.
  # config.vm.box_check_update = false


  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine. In the example below,
  # accessing "localhost:8080" will access port 80 on the guest machine.

  tomcatPort = ENV['TOMCAT_PORT']
  if tomcatPort.nil?
    tomcatPort = '8888'
  end

  transPort = ENV['NODEJS_PORT']
  if transPort.nil?
    transPort = '8094'
  end

  mergePort = ENV['P2P_PORT']
  if mergePort.nil?
    mergePort = '8096'
  end

  mapnikPort = ENV['NODE_MAPNIK_SERVER_PORT']
  if mapnikPort.nil?
    mapnikPort = '8000'
  end

  # tomcat service
  config.vm.network "forwarded_port", guest: 8080, host: tomcatPort
  # translation nodejs service
  config.vm.network "forwarded_port", guest: 8094, host: transPort
  # merge nodejs service
  config.vm.network "forwarded_port", guest: 8096, host: mergePort
  # node-mapnik-server nodejs service
  config.vm.network "forwarded_port", guest: 8000, host: mapnikPort

  # Create a private network, which allows host-only access to the machine
  # using a specific IP.
  # config.vm.network "private_network", ip: "192.168.33.11"

  # Create a public network, which generally matched to bridged network.
  # Bridged networks make the machine appear as another physical device on
  # your network.
  # config.vm.network "public_network"

  # Share an additional folder to the guest VM. The first argument is
  # the path on the host to the actual folder. The second argument is
  # the path on the guest to mount the folder. And the optional third
  # argument is a set of non-required options.
  config.vm.synced_folder ".", "/home/vagrant/hoot"

  # Provider-specific configuration so you can fine-tune various
  # backing providers for Vagrant. These expose provider-specific options.
  # Example for VirtualBox:
  #
  config.vm.provider "virtualbox" do |vb|
  #   # Display the VirtualBox GUI when booting the machine
  #   vb.gui = true
  #
  #   # Customize the amount of memory on the VM:
     vb.memory = 8192
     vb.cpus = 4
  end

  # This is a provider for the Parallels Virtualization Software
  # Run "vagrant up --provider=parallels" to spin up using parallels.
  config.vm.provider "parallels" do |para, override|
        para.memory = 8192
        para.cpus = 4
        override.vm.box = "parallels/ubuntu-14.04"
        override.vm.box_url = "https://atlas.hashicorp.com/parallels/boxes/ubuntu-14.04"
  end

  # This is a provider for VMware Workstation
  # Run "vagrant up --provider=vmware_workstation" to spin up using VMware.
  config.vm.provider "vmware_workstation" do |vw, override|
      vw.memory = 8192
      vw.cpus = 4
      override.vm.box = "puphpet/ubuntu1404-x64"
      override.vm.box_url = "https://atlas.hashicorp.com/puphpet/boxes/ubuntu1404-x64"
  end
  #
  # View the documentation for the provider you are using for more
  # information on available options.

  # Define a Vagrant Push strategy for pushing to Atlas. Other push strategies
  # such as FTP and Heroku are also available. See the documentation at
  # https://docs.vagrantup.com/v2/push/atlas.html for more information.
  # config.push.define "atlas" do |push|
  #   push.app = "YOUR_ATLAS_USERNAME/YOUR_APPLICATION_NAME"
  # end

  # Enable provisioning with a shell script. Additional provisioners such as
  # Puppet, Chef, Ansible, Salt, and Docker are also available. Please see the
  # documentation for more information about their specific syntax and use.
  # config.vm.provision "shell", inline: <<-SHELL
  #   sudo apt-get update
  #   sudo apt-get install -y apache2
  # SHELL
  config.vm.provision "hoot", type: "shell", :privileged => false, :path => "VagrantProvision.sh"
  config.vm.provision "build", type: "shell", :privileged => false, :path => "VagrantBuild.sh"
  config.vm.provision "tomcat", type: "shell", :privileged => false, :inline => "sudo service tomcat6 restart", run: "always"
  config.vm.provision "mapnik", type: "shell", :privileged => false, :inline => "sudo service node-mapnik-server start", run: "always"
  config.vm.provision "hadoop", type: "shell", :privileged => false, :inline => "stop-all.sh && start-all.sh", run: "always"

end

# Allow local overrides of vagrant settings
if File.exists?('VagrantfileLocal')
  load 'VagrantfileLocal'
else
  if File.exists?('VagrantfileLocal.vbox')
    load 'VagrantfileLocal.vbox'
  end
end


