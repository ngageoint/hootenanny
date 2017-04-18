#!/usr/bin/env bash

#size in megabytes; 3072 is recommended if you will be running hoot core tests; 
# 4096 recommended if you will be using valgrind with hoot core
DISK_SIZE=$1
if [ ! -d /ramdisk/hoot ]; then
  echo "Creating hoot RAM disk..."
  sudo mkdir -p /ramdisk 
  sudo mount -t tmpfs -o size=$1M tmpfs /ramdisk
  cp -R ~/hoot /ramdisk
else
  echo "Hoot RAM disk already created."
fi
