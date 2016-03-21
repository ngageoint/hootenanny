#!/bin/bash

cd $HOOT_HOME && aclocal && autoconf && autoheader && automake && ./configure -q --with-rnd && make -sj$(nproc)
