#!/usr/bin/env bash

if [ ! -d /ramdisk/hoot ]; then
  echo "Creating hoot RAM disk..."
  sudo mkdir -p /ramdisk 
  sudo mount -t tmpfs -o size=4096M tmpfs /ramdisk
  cp -R ~/hoot /ramdisk
else
  echo "Hoot RAM disk already created."
fi
