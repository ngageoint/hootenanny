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
      if os == 'CentOS7'
        override.vm.provision 'hoot', type: 'shell', :privileged => false, :path => 'VagrantProvisionCentOS7.sh'
        tomcat_script = 'sudo systemctl restart tomcat8'
#        mapnik_script = 'sudo systemctl restart node-mapnik'
      end

      override.vm.provision 'build', type: 'shell', :privileged => false, :path => 'VagrantBuild.sh'
      override.vm.provision 'EGD', type: 'shell', :privileged => false, :inline  => '([ -f ~/ActivateEGDplugin.sh ] && sudo -u tomcat ~/ActivateEGDplugin.sh /var/lib/tomcat8) || true'
      override.vm.provision 'tomcat', type: 'shell', :privileged => false, :inline => tomcat_script, run: 'always'
#      override.vm.provision 'mapnik', type: 'shell', :privileged => false, :inline => mapnik_script, run: 'always'

      # TODO: Why is node-export only on CentOS?
      if os.start_with?('CentOS')
        override.vm.provision 'export', type: 'shell', :privileged => false, :inline => 'sudo systemctl restart node-export'
      end
    end
  end

  # Centos7 box - Preprovisioned for compiling hootenanny
  config.vm.define "default", primary: true do |hoot_centos7_prov|
    hoot_centos7_prov.vm.box = "hoot/centos7-hoot"
    hoot_centos7_prov.vm.hostname = "centos7-hoot"
    hoot_centos7_prov.vm.synced_folder ".", "/home/vagrant/hoot"

    hoot_centos7_prov.vm.provision "hoot", type: "shell", :privileged => false, :path => "VagrantProvisionCentOS7.sh"
    hoot_centos7_prov.vm.provision "build", type: "shell", :privileged => false, :path => "VagrantBuild.sh"
    hoot_centos7_prov.vm.provision "tomcat", type: "shell", :privileged => false, :inline => "sudo systemctl restart tomcat8", run: "always"
#    hoot_centos7_prov.vm.provision "mapnik", type: "shell", :privileged => false, :inline => "sudo systemctl restart node-mapnik", run: "always"
    hoot_centos7_prov.vm.provision "export", type: "shell", :privileged => false, :inline => "sudo systemctl restart node-export", run: "always"

    aws_provider(hoot_centos7_prov, 'CentOS7')
  end

  # Centos7 box - not preprovisioned
  config.vm.define "hoot_centos7", autostart: false do |hoot_centos7|
    hoot_centos7.vm.box = "hoot/centos7-minimal"
    hoot_centos7.vm.hostname = "hoot-centos"
    hoot_centos7.vm.synced_folder ".", "/home/vagrant/hoot"

    hoot_centos7.vm.provision "hoot", type: "shell", :privileged => false, :path => "VagrantProvisionCentOS7.sh"
    hoot_centos7.vm.provision "build", type: "shell", :privileged => false, :path => "VagrantBuild.sh"
    hoot_centos7.vm.provision "tomcat", type: "shell", :privileged => false, :inline => "sudo systemctl restart tomcat8", run: "always"
#    hoot_centos7.vm.provision "mapnik", type: "shell", :privileged => false, :inline => "sudo systemctl restart node-mapnik", run: "always"
    hoot_centos7.vm.provision "export", type: "shell", :privileged => false, :inline => "sudo systemctl restart node-export", run: "always"

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
#    dockcentos72.vm.provision "mapnik", type: "shell", :privileged => false, :inline => "sudo systemctl restart node-mapnik", run: "always"
    dockcentos72.vm.provision "export", type: "shell", :privileged => false, :inline => "sudo systemctl restart node-export", run: "always"
  end

  # Provider-specific configuration so you can fine-tune various
  # backing providers for Vagrant. These expose provider-specific options.
  # Example for VirtualBox:
  #
  config.vm.provider "virtualbox" do |vb|
    # Display the VirtualBox GUI when booting the machine
    #vb.gui = true

  # Customize the amount of memory on the VM:
    vb.memory = 10240
    vb.cpus = 4
  end

  # VSphere provider
  config.vm.provider "vsphere" do |vsphere, override|
    override.vm.box     = 'VSphereDummy'
    override.vm.box_url = 'VSphereDummy.box'
    vsphere.insecure    = true
    vsphere.cpu_count   = 8
    vsphere.memory_mb   = 16384
    if ENV.key?('VSPHERE_HOST')
      vsphere.host = ENV['VSPHERE_HOST']
    end
    if ENV.key?('VSPHERE_RESOURCE')
      vsphere.compute_resource_name = ENV['VSPHERE_RESOURCE']
    end
    if ENV.key?('VSPHERE_PATH')
      vsphere.vm_base_path = ENV['VSPHERE_PATH']
    end
    if ENV.key?('VSPHERE_TEMPLATE')
      vsphere.template_name = ENV['VSPHERE_TEMPLATE']
    end
    if ENV.key?('VSPHERE_USER')
      vsphere.user = ENV['VSPHERE_USER']
    end
    if ENV.key?('VSPHERE_PASSWORD')
      vsphere.password = ENV['VSPHERE_PASSWORD']
    end
    if ENV.key?('VSPHERE_NAME')
      vsphere.name = ENV['VSPHERE_NAME']
    end
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
