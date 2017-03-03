#!/usr/bin/env bash

if [ ! -d /fouo ]; then
  echo "Creating regression data symbolic link..."
  sudo ln -s /home/vagrant/fouo /fouo
else
  echo "Regression data symbolic link exists."
fi
