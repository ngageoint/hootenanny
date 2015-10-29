#!/bin/bash
# EZ script for cleaning and building hoot in one shot without services

cd $HOOT_HOME
make -sj8 clean
aclocal && autoconf && autoheader && automake
./configure --with-rnd --with-services && make -sj8
