#!/bin/bash
# EZ button for configuring and building with nodejs

cd $HOOT_HOME
aclocal && autoconf && autoheader && automake && ./configure -q --with-rnd --with-services && make -sj8
