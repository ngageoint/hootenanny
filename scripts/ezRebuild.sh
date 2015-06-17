#!/bin/bash
# EZ script for cleaning and building hoot in one shot without services

cd $HOOT_HOME
aclocal && autoconf && autoheader && automake && ./configure && make -sj8 clean-all
./configure -q --with-rnd && make -sj8
