#!/bin/bash
#
location=$HOME/gitdir/qsim/build/output/
cd $location

for i in `seq 1 100`;
do
	name="out_$i"
	mkdir $name
	cd $name
	qsub ../../macros/runscript.sh
	cd ..
done
