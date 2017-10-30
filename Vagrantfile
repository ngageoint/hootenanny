# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure(2) do |config|
  # Hoot port mapping
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

  disableForwarding = ENV['DISABLE_VAGRANT_FORWARDING']
  if disableForwarding.nil?
    # tomcat service
    config.vm.network "forwarded_port", guest: 8080, host: tomcatPort
    # translation nodejs service
    config.vm.network "forwarded_port", guest: 8094, host: transPort
    # merge nodejs service
    config.vm.network "forwarded_port", guest: 8096, host: mergePort
    # node-mapnik-server nodejs service
    config.vm.network "forwarded_port", guest: 8000, host: mapnikPort
  end

  def aws_provider(config, os)
    # AWS Provider.  Set enviornment variables for values below to use
    config.vm.provider :aws do |aws, override|
      override.nfs.functional = false
      aws.instance_type = ENV.fetch('AWS_INSTANCE_TYPE', 'm3.xlarge')
      aws.block_device_mapping = [{ 'DeviceName' => '/dev/sda1', 'Ebs.VolumeSize' => 32 }]

      # By default we assume that we're using the AWS "dummy" box and manually
      # specify an Ubuntu AMI.  These assumptions will be removed when we migrate
      # to boxes that have native AWS providers that use the standard Vagrant key.
      if os == 'Ubuntu1404'
        override.vm.box = 'dummmy'
        override.ssh.private_key_path = ENV['AWS_PRIVATE_KEY_PATH']
        aws.ami = ENV['AWS_AMI_UBUNTU1404']
      end

      if ENV.key?('AWS_KEYPAIR_NAME')
        aws.keypair_name = ENV['AWS_KEYPAIR_NAME']
      end

      if ENV.key?('AWS_SECURITY_GROUP')
        aws.security_groups = ENV['AWS_SECURITY_GROUP']
      end

      aws.tags = {
        'Name' => ENV.fetch('AWS_INSTANCE_NAME_TAG', "jenkins-hootenanny-#{os.downcase}"),
        'URL'  => ENV.fetch('AWS_INSTANCE_URL_TAG', 'https://github.com/ngageoint/hootenanny'),
      }

      if ENV.key?('JOB_NAME')
        aws.tags['JobName'] = ENV['JOB_NAME']
      end

      if ENV.key?('BUILD_NUMBER')
        aws.tags['BuildNumber'] = ENV['BUILD_NUMBER']
      end

      # Copy over predownloaded packages if they are found
      override.vm.provision 'software', type: 'shell', run: 'always', :inline => '( [ -d /home/vagrant/hoot/software ] && cp /home/vagrant/hoot/software/* /home/vagrant ) || true'

      # Setting up provisioners for AWS, in the correct order, depending on the OS platform.
      if os == 'Ubuntu1404'
        override.vm.provision 'hoot', type: 'shell', :privileged => false, :path => 'VagrantProvision.sh'
        tomcat_script = 'sudo service tomcat8 restart'
        mapnik_script = 'sudo service node-mapnik-server start'
      elsif os == 'Ubuntu1604'
        override.vm.provision 'hoot', type: 'shell', :privileged => false, :path => 'VagrantProvision1604.sh'
        # TODO: Doesn't 16.04 use systemd too?
        tomcat_script = 'sudo service tomcat8 restart'
        mapnik_script = 'sudo service node-mapnik-server start'
      elsif os == 'CentOS7'
        override.vm.provision 'hoot', type: 'shell', :privileged => false, :path => 'VagrantProvisionCentOS7.sh'
        tomcat_script = 'sudo systemctl restart tomcat8'
        mapnik_script = 'sudo systemctl restart node-mapnik'
      end

      override.vm.provision 'build', type: 'shell', :privileged => false, :path => 'VagrantBuild.sh'
      override.vm.provision 'EGD', type: 'shell', :privileged => false, :inline  => '([ -f ~/ActivateEGDplugin.sh ] && sudo -u tomcat8 ~/ActivateEGDplugin.sh /var/lib/tomcat8) || true'
      override.vm.provision 'tomcat', type: 'shell', :privileged => false, :inline => tomcat_script, run: 'always'
      override.vm.provision 'mapnik', type: 'shell', :privileged => false, :inline => mapnik_script, run: 'always'

      # TODO: Why is node-export only on CentOS?
      if os.start_with?('CentOS')
        override.vm.provision 'export', type: 'shell', :privileged => false, :inline => 'sudo systemctl restart node-export'
      end

      override.vm.provision 'hadoop', type: 'shell', :privileged => false, :inline => 'stop-all.sh && start-all.sh', run: 'always'
    end
  end

  # Global settings - default for Ubuntu1404

  # Ubuntu1404 Box
  # This is the standard, working box
  config.vm.define "default", primary: true do |hoot|
    hoot.vm.box = "ubuntu/trusty64"
    hoot.vm.box_url = "https://atlas.hashicorp.com/ubuntu/boxes/trusty64"
    hoot.vm.synced_folder ".", "/home/vagrant/hoot"
    hoot.vm.provision "hoot", type: "shell", :privileged => false, :path => "VagrantProvision.sh"
    hoot.vm.provision "build", type: "shell", :privileged => false, :path => "VagrantBuild.sh"
    hoot.vm.provision "tomcat", type: "shell", :privileged => false, :inline => "sudo service tomcat8 restart", run: "always"
    hoot.vm.provision "mapnik", type: "shell", :privileged => false, :inline => "sudo service node-mapnik-server start", run: "always"
    hoot.vm.provision "hadoop", type: "shell", :privileged => false, :inline => "stop-all.sh && start-all.sh", run: "always"

    aws_provider(hoot, 'Ubuntu1404')
  end

  config.vm.define "docker14.04", autostart: false do |dock1404|
    dock1404.vm.box = "tknerr/baseimage-ubuntu-14.04"
    dock1404.vm.box_version = "1.0.0"
    #dock1404.vm.box_url = "https://atlas.hashicorp.com/ubuntu/boxes/trusty64"
    dock1404.vm.synced_folder ".", "/home/vagrant/hoot"

    dock1404.vm.provider "docker" do |docker|
        #docker.create_args = ['--security-opt', 'seccomp=./chrome.json']
        docker.create_args = ['--privileged','--shm-size', '1g']
    end

    dock1404.vm.provision "hoot", type: "shell", :privileged => false, :path => "VagrantProvision.sh"
    dock1404.vm.provision "build", type: "shell", :privileged => false, :path => "VagrantBuild.sh"
    dock1404.vm.provision "tomcat", type: "shell", :privileged => false, :inline => "sudo service tomcat8 restart", run: "always"
    dock1404.vm.provision "mapnik", type: "shell", :privileged => false, :inline => "sudo service node-mapnik-server start", run: "always"
    dock1404.vm.provision "hadoop", type: "shell", :privileged => false, :inline => "stop-all.sh && start-all.sh", run: "always"
  end

  config.vm.define "dockercentos7.2", autostart: false do |dockcentos72|

    dockcentos72.ssh.insert_key = false
    dockcentos72.vm.provider "docker" do |d|
      d.image = "local/centos7.2vagrant"
      d.create_args = ['-td','--privileged','--shm-size','1g']
      d.has_ssh = true
    end

    dockcentos72.vm.synced_folder ".", "/home/vagrant/hoot"

    # Stop the default vagrant rsyncing
    config.vm.synced_folder '.', '/home/vagrant/sync', disabled: true

    dockcentos72.vm.provision "hoot", type: "shell", :privileged => false, :path => "VagrantProvisionCentOS7.sh"
    dockcentos72.vm.provision "build", type: "shell", :privileged => false, :path => "VagrantBuild.sh"

    dockcentos72.vm.provision "tomcat", type: "shell", :privileged => false, :inline => "sudo systemctl restart tomcat8", run: "always"
    dockcentos72.vm.provision "mapnik", type: "shell", :privileged => false, :inline => "sudo systemctl restart node-mapnik", run: "always"
    dockcentos72.vm.provision "export", type: "shell", :privileged => false, :inline => "sudo systemctl restart node-export", run: "always"
    dockcentos72.vm.provision "hadoop", type: "shell", :privileged => false, :inline => "stop-all.sh && start-all.sh", run: "always"


  end


  # Ubuntu1604 Box


  # Ubuntu1604 Box
  # For testing before we upgrade from Ubuntu1404
  config.vm.define "hoot_ubuntu1604", autostart: false do |hoot_ubuntu1604|
    #hoot_ubuntu1604.vm.box = "ubuntu/xenial64"
    #hoot_ubuntu1604.vm.box_url = "https://atlas.hashicorp.com/ubuntu/boxes/xenial64"

    # Why does this box have an ubuntu user, not a vagrant user?   Why?????

    hoot_ubuntu1604.vm.synced_folder ".", "/home/vagrant/hoot"
    # This should work for libvirt and virtualbox.
    hoot_ubuntu1604.vm.box = "elastic/ubuntu-16.04-x86_64"
    hoot_ubuntu1604.vm.box_url = "https://app.vagrantup.com/elastic/boxes/ubuntu-16.04-x86_64"

    hoot_ubuntu1604.vm.provision "hoot", type: "shell", :privileged => false, :path => "VagrantProvisionUbuntu1604.sh"
    hoot_ubuntu1604.vm.provision "build", type: "shell", :privileged => false, :path => "VagrantBuild.sh"
    #hoot_ubuntu1604.vm.provision "tomcat", type: "shell", :privileged => false, :inline => "sudo service tomcat8 restart", run: "always"
    #hoot_ubuntu1604.vm.provision "mapnik", type: "shell", :privileged => false, :inline => "sudo service node-mapnik-server start", run: "always"
    #hoot_ubuntu1604.vm.provision "hadoop", type: "shell", :privileged => false, :inline => "stop-all.sh && start-all.sh", run: "always"
  end

  # Centos7 box
  config.vm.define "hoot_centos7", autostart: false do |hoot_centos7|
    hoot_centos7.vm.box = "hoot/centos7-minimal"
    hoot_centos7.vm.hostname = "hoot-centos"
    hoot_centos7.vm.synced_folder ".", "/home/vagrant/hoot"

    hoot_centos7.vm.provision "hoot", type: "shell", :privileged => false, :path => "VagrantProvisionCentOS7.sh"
    hoot_centos7.vm.provision "build", type: "shell", :privileged => false, :path => "VagrantBuild.sh"
    hoot_centos7.vm.provision "tomcat", type: "shell", :privileged => false, :inline => "sudo systemctl restart tomcat8", run: "always"
    hoot_centos7.vm.provision "mapnik", type: "shell", :privileged => false, :inline => "sudo systemctl restart node-mapnik", run: "always"
    hoot_centos7.vm.provision "export", type: "shell", :privileged => false, :inline => "sudo systemctl restart node-export", run: "always"
    hoot_centos7.vm.provision "hadoop", type: "shell", :privileged => false, :inline => "stop-all.sh && start-all.sh", run: "always"

    aws_provider(hoot_centos7, 'CentOS7')
  end


  # Centos7 - Hoot core ONLY. No UI
  config.vm.define "hoot_centos7_core", autostart: false do |hoot_centos7_core|
    # Turn off port forwarding
    config.vm.network "forwarded_port", guest: 8080, host: tomcatPort, disabled: true
    config.vm.network "forwarded_port", guest: 8094, host: transPort, disabled: true
    config.vm.network "forwarded_port", guest: 8096, host: mergePort, disabled: true
    config.vm.network "forwarded_port", guest: 8000, host: mapnikPort, disabled: true

    hoot_centos7_core.vm.box = "bento/centos-7.2"
    hoot_centos7_core.vm.box_url = "https://atlas.hashicorp.com/bento/boxes/centos-7.2"

    # Stop the default vagrant rsyncing
    config.vm.synced_folder '.', '/home/vagrant/sync', disabled: true

    # Use the plugin to install bindfs and make a dummy mount
    hoot_centos7_core.bindfs.bind_folder "/tmp", "/home/vagrant/bindfstmp", perms: nil

    # Create an empty directory
    config.vm.synced_folder ".", "/vagrant/home/.workspace-nfs", type: "rsync", disabled: true

    hoot_centos7_core.vm.network "private_network", ip: "192.168.33.10"
    hoot_centos7_core.nfs.map_uid = Process.uid
    hoot_centos7_core.nfs.map_gid = Process.gid
    hoot_centos7_core.vm.synced_folder ".", "/home/vagrant/.hoot-nfs", type: "nfs", :linux__nfs_options => ['rw','no_subtree_check','all_squash','async']
    hoot_centos7_core.bindfs.bind_folder "/home/vagrant/.hoot-nfs", "/home/vagrant/hoot", perms: nil

    hoot_centos7_core.vm.provision "hoot", type: "shell", :privileged => false, :path => "scripts/util/Centos7_only_core.sh"
  end

  # Provider-specific configuration so you can fine-tune various
  # backing providers for Vagrant. These expose provider-specific options.
  # Example for VirtualBox:
  #
  config.vm.provider "virtualbox" do |vb|
  # Display the VirtualBox GUI when booting the machine
  #vb.gui = true

  # Customize the amount of memory on the VM:
#    vb.memory = 8192
    vb.memory = 10240
    vb.cpus = 4
  end

  # This is a provider for KVM
  # See https://github.com/pradels/vagrant-libvirt for install instructions
  # Run "vagrant up --provider=libvirt" to spin up using KVM.
  #
  # On Ubuntu 14.04 I had to use v0.0.35 of the vagrant-libvirt plugin. -JRS
  # https://github.com/vagrant-libvirt/vagrant-libvirt/issues/669
  #
  # vagrant plugin install --plugin-version 0.0.35 vagrant-libvirt
  # WARNING: Minimally tested
  config.vm.provider "libvirt" do |libvirt, override|
    override.nfs.map_uid = Process.uid
    override.nfs.map_gid = Process.gid
    # Disable the standard folders so we can use NFS
    override.vm.synced_folder '.', '/home/vagrant/sync', disabled: true
    override.vm.synced_folder '.', '/home/vagrant/hoot', disabled: true

    # Configure some fancy NFS syncing
    override.vm.synced_folder ".", "/home/vagrant/.hoot-nfs", type: "nfs",
      :mount_options => ['vers=3','udp','noatime','nodiratime','nocto', 'nolock'],
      :linux__nfs_options => ['rw','no_subtree_check','all_squash','async']
    override.bindfs.bind_folder "/home/vagrant/.hoot-nfs",
      "/home/vagrant/hoot",
      chgrp_ignore: true,
      chown_ignore: true,
      perms: nil
    libvirt.cpu_model = "host-passthrough"
    #override.vm.box = "s3than/trusty64"
    #override.vm.box_url = "https://app.vagrantup.com/s3than/boxes/trusty64"
    #override.vm.box = "iknite/trusty64"
    #override.vm.box_url = "https://app.vagrantup.com/iknite/boxes/trusty64"
    libvirt.memory = 8192
    libvirt.cpus = 8
  end

  # This is a provider for the Parallels Virtualization Software
  # Run "vagrant up --provider=parallels" to spin up using parallels.
  # WARNING: Minimally tested
  config.vm.provider "parallels" do |para, override|
        para.memory = 8192
        para.cpus = 4
        override.hoot.vm.box = "parallels/ubuntu-14.04"
        override.hoot.vm.box_url = "https://atlas.hashicorp.com/parallels/boxes/ubuntu-14.04"

        override.hoot_ubuntu1604.vm.box = "parallels/ubuntu-16.04"
        override.hoot_ubuntu1604.vm.box_url = "https://atlas.hashicorp.com/parallels/boxes/ubuntu-16.04"
        override.hoot_centos7.vm.box = "parallels/centos-7.3"
        override.hoot_centos7.vm.box_url = "https://atlas.hashicorp.com/parallels/boxes/centos-7.3"
  end

  # This is a provider for VMware Workstation
  # Run "vagrant up --provider=vmware_workstation" to spin up using VMware.
  # WARNING: Minimally tested
  config.vm.provider "vmware_workstation" do |vw, override|
      vw.memory = 8192
      vw.cpus = 4
      override.hoot.vm.box = "puphpet/ubuntu1404-x64"
      override.hoot.vm.box_url = "https://atlas.hashicorp.com/puphpet/boxes/ubuntu1404-x64"

      override.hoot_ubuntu1604.vm.box = "puphpet/ubuntu1604-x64"
      override.hoot_ubuntu1604.vm.box_url = "https://atlas.hashicorp.com/puphpet/boxes/ubuntu1604-x64"
  end

  # TODO: Add vSphere provider
end

# Allow local overrides of vagrant settings
if File.exists?('VagrantfileLocal')
  load 'VagrantfileLocal'
else
  if File.exists?('VagrantfileLocal.vbox')
    load 'VagrantfileLocal.vbox'
  end
end
