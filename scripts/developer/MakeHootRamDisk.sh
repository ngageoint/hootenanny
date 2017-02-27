#!/usr/bin/env bash

sudo mkdir -p /media/ramdisk 
sudo mount -t tmpfs -o size=2048M tmpfs /media/ramdisk
cp -R ~/hoot /media/ramdisk
