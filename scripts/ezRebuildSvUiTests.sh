#!/bin/bash

cd $HOOT_HOME && make -s clean && aclocal && autoconf && autoheader && automake --add-missing && ./configure --with-services --with-uitests && make -sj$(nproc)
