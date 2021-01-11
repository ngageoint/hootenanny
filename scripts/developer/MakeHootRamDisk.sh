#!/usr/bin/env bash

# Use this with VirtualBox VM's for much quicker hoot compiles.  Call from VagrantfileLocal like:
# config.vm.provision "hoot-ramdisk", type: "shell", :privileged => false, :inline => "/home/vagrant/hoot/scripts/developer/MakeHootRamDisk.sh 4096", run: "always"
# DISK_SIZE = size in megabytes; The size necessary seems to be environment dependent. If you run out of disk space, you'll 
# know you don't have enough. At a bare minimum: For running hoot core tests: 3072 is recommended on Ubuntu, 4096 on CentOS; 
# If you will be using valgrind with hoot core: 4096 is recommended on Ubuntu; 5120 on CentOS
DISK_SIZE=$1
if [ ! -d /ramdisk/hoot ]; then
  echo "Creating hoot RAM disk..."
  sudo mkdir -p /ramdisk 
  sudo mount -t tmpfs -o size=$1M tmpfs /ramdisk
  cp -R ~/hoot /ramdisk
  # This doesn't seem to help performance so far:
  #echo "Copying ccache..."
  #cp -R ~/.ccache /ramdisk
  #export CCACHE_DIR=/ramdisk/.ccache
else
  echo "Hoot RAM disk already created."
fi
