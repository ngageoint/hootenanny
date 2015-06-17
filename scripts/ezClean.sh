#!/bin/bash
# EZ button for cleaning build 

cd $HOOT_HOME
aclocal && autoconf && autoheader && automake && ./configure && make -sj8 clean-all
