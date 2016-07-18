#!/bin/bash

cd $HOOT_HOME && aclocal && autoconf && autoheader && automake --add-missing && ./configure -q --with-rnd && make -sj$(nproc)
