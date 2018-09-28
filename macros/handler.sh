#!/bin/bash
#

[ -f "batchexample.mac" ] || echo "!!!	batch example file not fount, please check it."
[ -f "batchrun.sh" ] || echo "!!!	batch run script not fount, please check it."

for i in `seq 1 100`;
do
    name="out_$i"
    mkdir $name
    cp "batchexample.mac" "$name/batch.mac"
    cp "batchrun.sh" "$name/run.sh"
    pushd $name
    sed -i "/OUTPUT/s/OUTPUT/out_$i/" batch.mac
    sed -i "/SEED/s/SEED/$i/" batch.mac
    qsub -cwd run.sh
#    nohup time -o statics ../../qsim batch.mac &> out &
    popd
done
