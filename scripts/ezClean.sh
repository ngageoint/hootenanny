#!/bin/bash
# EZ button for cleaning build 

cd $HOOT_HOME
make -sj8 clean-all
