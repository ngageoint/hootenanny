#!/bin/bash
# EZ button for configuring and building with services

cd $HOOT_HOME
aclocal && autoconf && autoheader && automake && ./configure -q --with-services && make -sj8
