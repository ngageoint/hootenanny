#!/bin/bash
# EZ button for configuring and building

cd $HOOT_HOME
aclocal && autoconf && autoheader && automake && ./configure -q --with-rnd && make -sj8
