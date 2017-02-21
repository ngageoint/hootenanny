#!/bin/bash


IFS=';'

for i in $1; 
do 
	unzip "$2/$i.zip" -d "$2/$i"
done
unset IFS
