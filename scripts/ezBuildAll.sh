#!/bin/bash

cd $HOOT_HOME && aclocal && autoconf && autoheader && automake && ./configure -q --with-rnd --with-services --with-uitests && make -sj$(nproc)
