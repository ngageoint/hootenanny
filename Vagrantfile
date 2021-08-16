# -*- mode: ruby -*-
# vi: set ft=ruby :

# Allowing stuff to be set during "vagrant up"
# E.g.
# To use NFS, have 12CPU and 24Gb RAM
#  NFSSHARE=true VBCPU=12 VBRAM=24576 vagrant up
$nfsShare = ENV['NFSSHARE']
if $nfsShare.nil?
  $nfsShare = false
else
  puts '## Using NFS for file syncing'
end

$rsyncShare = ENV['RSYNCSHARE']
if $rsyncShare.nil?
  $rsyncShare = false
else
  puts '## Using RSYNC for file syncing'
end

$fouoShare = ENV['FOUOSHARE']
if $fouoShare.nil?
  $fouoShare = false
else
  puts '## Mounting /fouo on the VM'
end

$vbCpu = ENV['VBCPU']
if $vbCpu.nil?
  $vbCpu = 4
else
  puts "## Allocating #{$vbCpu} CPU cores to the VM"
end

$vbRam = ENV['VBRAM']
if $vbRam.nil?
  $vbRam = 10240
else
  puts "## Allocating #{$vbRam} RAM to the VM"
end

# This is only for RPM installs!
# For commandline only Hootenanny, set COREONLY to "yes". The default is a full install - core, services, UI etc
# E.g. COREONLY="yes" vagrant up hoot_centos7_rpm
$coreOnly = ENV['COREONLY']
if $coreOnly.nil?
  $coreOnly = "no"
else
  puts "## Installing commandline Hootenanny"
end

# Decide if we install the release version or the nightly development version
$nightly = ENV['NIGHTLY']
if $nightly.nil?
  $nightly = "no"
else
  puts "## Installing from the nightly RPMs"
end


# By default, don't try to add software repos in the VM's
# Not sure if this needs to be able to be triggered from the commandline
$addRepos = "no"

# Hootenanny port mapping
# If this is set, the VM will not forward any ports to the host
$disableForwarding = ENV['DISABLE_VAGRANT_FORWARDING']

$tomcatPort = ENV['TOMCAT_PORT']
if $tomcatPort.nil?
  $tomcatPort = 8888
end

$transPort = ENV['NODEJS_PORT']
if $transPort.nil?
  $transPort = 8094
end

$mergePort = ENV['P2P_PORT']
if $mergePort.nil?
  $mergePort = 8096
end

$nodeExportPort = ENV['NODE_EXPORT_PORT']
if $nodeExportPort.nil?
  $nodeExportPort = 8101
end

$yumUpdate = ENV['YUMUPDATE']
if $yumUpdate.nil?
  $yumUpdate = "no"
end

Vagrant.configure(2) do |config|

  def aws_provider(config, os, vbox)
    # AWS Provider.  Set enviornment variables for values below to use
    config.vm.provider :aws do |aws, override|
      override.nfs.functional = false
      if vbox == "hoot"
          aws.region_config ENV['AWS_DEFAULT_REGION'], :ami => ENV['AWS_HOOT_AMI_ID']
      elsif vbox == "minimal"
          aws.region_config ENV['AWS_DEFAULT_REGION'], :ami => ENV['AWS_MINIMAL_AMI_ID']
      end
      aws.subnet_id = ENV['AWS_SUBNET_ID']
      if ENV.key?('AWS_KEYPAIR_NAME')
        aws.keypair_name = ENV['AWS_KEYPAIR_NAME']
      end
      if ENV.key?('AWS_SECURITY_GROUP')
        $security_grp = ENV['AWS_SECURITY_GROUP']
        if $security_grp.is_a?(String) and $security_grp.include? ',' and $security_grp.split(',').length > 0
            aws.security_groups = $security_grp.split(',')
        else
            aws.security_groups = $security_grp
        end
      end

      aws.instance_type = ENV.fetch('AWS_INSTANCE_TYPE', 'm5.2xlarge')
      aws.block_device_mapping = [{ 'DeviceName' => '/dev/sda1', 'Ebs.VolumeSize' => 64 }]

      aws.tags = {
        'Name' => ENV.fetch('AWS_INSTANCE_NAME_TAG', "jenkins-hootenanny-#{os.downcase}"),
        'URL'  => ENV.fetch('AWS_INSTANCE_URL_TAG', 'https://github.com/ngageoint/hootenanny'),
        'env' => ENV.fetch('HOOT_AWS_ENV_TAG', 'testing'),
        'use' => ENV.fetch('HOOT_AWS_USE_TAG', 'Jenkins'),
        'group' => ENV.fetch('HOOT_AWS_GROUP_TAG', 'devops')
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
        override.vm.provision "hoot", type: "shell", :privileged => false, :path => "VagrantProvisionCentOS7.sh", :env => {"ADDREPOS" => $addRepos, "YUMUPDATE" => $yumUpdate}
        # override.vm.provision 'hoot', type: 'shell', :privileged => false, :path => 'VagrantProvisionCentOS7.sh'
        tomcat_script = 'sudo systemctl restart tomcat8'
      end

      override.vm.provision 'build', type: 'shell', :privileged => false, :path => 'VagrantBuild.sh'
      override.vm.provision 'EGD', type: 'shell', :privileged => false, :inline  => '([ -f ~/ActivateEGDplugin.sh ] && sudo -u tomcat ~/ActivateEGDplugin.sh /var/lib/tomcat8) || true'
      override.vm.provision 'tomcat', type: 'shell', :privileged => false, :inline => tomcat_script, run: 'always'

      # TODO: Why is node-export only on CentOS?
      if os.start_with?('CentOS')
        override.vm.provision 'export', type: 'shell', :privileged => false, :inline => 'sudo systemctl restart node-export'
      end
    end
  end

  def mount_shares(config)
    # sharing of the hosts hoot folder and optionally the fouo folder with or without nfs
    if $nfsShare
      config.vm.network "private_network", ip: "192.168.33.10"
      config.vm.synced_folder ".", "/home/vagrant/hoot", type: "nfs", mount_options: ['vers=4'], nfs_version: 4
      if $fouoShare
        config.vm.synced_folder "/fouo", "/fouo", type: "nfs"
      end
    elsif $rsyncShare
      config.vm.synced_folder ".", "/home/vagrant/hoot", type: "rsync", rsync__exclude: ['.vagrant/', '.git/']
      if $fouoShare
        config.vm.synced_folder "/fouo", "/fouo", type: "rsync"
      end
    else
      # Use sshfs sharing if available, otherwise default sharing
      if Vagrant.has_plugin?("vagrant-sshfs")
        config.vm.synced_folder ".", "/home/vagrant/hoot", type: "sshfs", sshfs_opts_append: "-o nonempty"
        if $fouoShare
          config.vm.synced_folder "/fouo", "/fouo", type: "sshfs", sshfs_opts_append: "-o nonempty"
        end
      else
        config.vm.synced_folder ".", "/home/vagrant/hoot"
        if $fouoShare
          config.vm.synced_folder "/fouo", "/fouo"
        end
      end
    end
  end

  def set_provisioners(config)
    config.vm.provision "hoot", type: "shell", :privileged => false, :path => "VagrantProvisionCentOS7.sh", :env => {"ADDREPOS" => $addRepos, "YUMUPDATE" => $yumUpdate}
    config.vm.provision "build", type: "shell", :privileged => false, :path => "VagrantBuild.sh"
    config.vm.provision "tomcat", type: "shell", :privileged => false, :inline => "sudo systemctl restart tomcat8", run: "always"
    config.vm.provision "export", type: "shell", :privileged => false, :inline => "sudo systemctl restart node-export", run: "always"
    config.vm.provision "valgrind", type: "shell", :privileged => false, :path => "scripts/valgrind/valgrind_install.sh"
  end

  def set_forwarding(config)
    if $disableForwarding.nil?
      config.vm.network "forwarded_port", guest: 8080, host: $tomcatPort  # Tomcat service
      config.vm.network "forwarded_port", guest: 8094, host: $transPort  # NodeJS Translation service
      config.vm.network "forwarded_port", guest: 8096, host: $mergePort  # NodeJS Merge service
      config.vm.network "forwarded_port", guest: 8101, host: $nodeExportPort  # NodeJS export service
    end
  end

  # Centos7 Hootenanny box from RPM's
  config.vm.define "hoot_centos7_rpm", autostart: false do |hoot_centos7_rpm|
    hoot_centos7_rpm.vm.box = "hoot/centos7-minimal"
    hoot_centos7_rpm.vm.hostname = "hoot-centos7-rpm"

    set_forwarding(hoot_centos7_rpm)
    mount_shares(hoot_centos7_rpm)

    # NOTE: For commandline only Hootenanny, set COREONLY to "yes"
    hoot_centos7_rpm.vm.provision "hootrpm", type: "shell", :privileged => false, :path => "VagrantProvisionCentOS7Rpm.sh", :env => {"YUMUPDATE" => $yumUpdate, "COREONLY" => $coreOnly, "NIGHTLY" => $nightly}
  end

  # Centos7 box - Preprovisioned for compiling hootenanny
  config.vm.define "default", primary: true do |hoot_centos7_prov|
    hoot_centos7_prov.vm.box = "hoot/centos7-hoot"
    hoot_centos7_prov.vm.hostname = "centos7-hoot"

    set_forwarding(hoot_centos7_prov)
    mount_shares(hoot_centos7_prov)
    set_provisioners(hoot_centos7_prov)
    aws_provider(hoot_centos7_prov, 'CentOS7', 'hoot')
  end

  # Centos7 box - not preprovisioned
  config.vm.define "hoot_centos7", autostart: false do |hoot_centos7|
    hoot_centos7.vm.box = "hoot/centos7-minimal"
    hoot_centos7.vm.hostname = "hoot-centos"

    set_forwarding(hoot_centos7)
    mount_shares(hoot_centos7)

    # We do want to add repos and update this box
    # NOTE: This change applies to every target AFTER this one as well.
    $addRepos = "yes"
    $yumUpdate = "yes"
    set_provisioners(hoot_centos7)
    aws_provider(hoot_centos7, 'CentOS7', 'minimal')
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
    dockcentos72.vm.provision "export", type: "shell", :privileged => false, :inline => "sudo systemctl restart node-export", run: "always"
  end

  # Provider-specific configuration so you can fine-tune various
  # backing providers for Vagrant. These expose provider-specific options.
  config.vm.provider "virtualbox" do |vb|
    # Customize the amount of memory on the VM:
    vb.memory = $vbRam
    vb.cpus = $vbCpu
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
