#!/bin/bash

cd $HOOT_HOME && aclocal && autoconf && autoheader && automake && ./configure -q --with-services --with-uitests && make -sj$(nproc)
