#!/bin/bash

export CCACHE_DISABLE=1
cd ../hoot-core

qmake hoot-core.pro; make -f Makefile.qmake clean

time( qmake hoot-core.pro; make -f Makefile.qmake -sj8 )

export CCACHE_DISABLE=0
