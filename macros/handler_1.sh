#!/bin/bash
#

[ -f "batchexample.mac" ] || echo "!!!	batch example file not fount, please check it."

for i in `seq 1 100`;
do
    name="out_$i"
    mkdir $name
    cp "batchexample.mac" "$name/batch.mac"
    pushd $name
    sed -i "/OUTPUT/s/OUTPUT/out_$i/" batch.mac
    sed -i "/SEED/s/SEED/$i/" batch.mac
    nohup time -o statics ../../qsim batch.mac &> out &
    popd
done
