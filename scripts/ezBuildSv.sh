#!/bin/bash

cd $HOOT_HOME && aclocal && autoconf && autoheader && automake && ./configure -q --with-services && make -sj$(nproc)
