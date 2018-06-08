#!/usr/bin/env bash
set -e

# Configure the make
aclocal && autoconf && autoheader && automake --add-missing --copy
./configure --quiet --with-rnd --with-services --with-uitests --with-coverage

# Make with the sonar build watcher
build-wrapper-linux-x86-64 --out-dir bw-output make -sj$(nproc)
