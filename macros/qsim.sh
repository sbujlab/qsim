#!/bin/bash

# Takes 5 arguments
# 
# Fixed value of non-scanned variable - default = 0.0
# Variable to scan ("angle" or "x")   - default = "angle"
# Min of scanned variable             - default = -30
# Max of scanned variable             - default = 30
# Step size                           - default = 0.5

if [ "$#" -lt 1 ] ; then
    echo "  ERROR, requires at least one input
    "
    echo "  usage: ./qsim.sh fixed-non-scanned \"variable-to-scan\" min-of-scan (-30) max-of-scan (30) step-size (0.5)
        Takes 9 arguments

        Fixed value of non-scanned variable - default = 0.0
        Variable to scan (\"angle\" or \"x\")   - default = \"angle\"
        Min of scanned variable             - default = -30
        Max of scanned variable             - default = 30
        Step size                           - default = 0.5
        Reflectivity of light guide         - default = 0.9
        Cerenkov                            - default = 1.0
        Scintillation                       - default = 1.0
        z Position of beam origin           - default = -11.0"
    exit
fi
fixed="$1"
scanned="angle"
scanMin=-30.0
scanMax=30.0
scanStep=0.5
reflectivity=0.9
cerenkov=1
scintillation=1
#z_pos=-52.0
z_pos=-11.0
if [ "$#" -gt 1 ] ; then
    scanned="$2"
fi
if [ "$#" -gt 2 ] ; then
    scanMin=$3
fi
if [ "$#" -gt 3 ] ; then
    scanMax=$4
fi
if [ "$#" -gt 4 ] ; then
    scanStep=$5
fi
if [ "$#" -gt 5 ] ; then
    reflectivity=$6
fi
if [ "$#" -gt 6 ] ; then
    cerenkov=$(printf "%.0f" "$7")
fi
if [ "$#" -gt 7 ] ; then
    scintillation=$(printf "%.0f" "$8")
fi
if [ "$#" -gt 8 ] ; then
    z_pos=$9
fi

numSteps=$(printf "%.0f" "$(bc -l <<< \(${scanMax}-\(${scanMin}\)\)/$scanStep)")
fixed=$(printf "%.1f" "$(bc -l <<< 1.0*$fixed)")
z_point=-11.0

defaultName="qsimout.root"
outputName="qsimout.root"
name="0.0_degrees_0.0_x"

for currentStep in `seq 0 $numSteps`;
do
    point=$(printf "%.1f" "$(bc -l <<< \($scanMin+\(1.0*$currentStep*$scanStep\)\))")
    if [[ "$scanned" == "angle" ]] ; then
        name="${point}_degrees_${fixed}_x_${z_pos}_z_${reflectivity}_ref_${cerenkov}_cer_${scintillation}_scint"
        cp preserve.mac sim_${name}.mac
        sed -i 's;'"thetamin 0.0 deg"';'"thetamin ${point} deg"';g' sim_${name}.mac
        sed -i 's;'"thetamax 0.0 deg"';'"thetamax ${point} deg"';g' sim_${name}.mac
        sed -i 's;'"xmin 0.0 cm"';'"xmin ${fixed} cm"';g' sim_${name}.mac
        sed -i 's;'"xmax 0.0 cm"';'"xmax ${fixed} cm"';g' sim_${name}.mac
        # 0.1994 = sin(11.5 degrees), $fixed is the distance from 0.0, + is farther towards PMT, further back away from +z axis
        z_point=$(printf "%.1f" "$(bc -l <<< ${z_pos}-\($fixed*0.1994\))")
        #z_point=$(printf "%.1f" "$(bc -l <<< -11.0-\($fixed*0.1994\))")
    fi
    if [[ "$scanned" == "x" ]] ; then
        name="${fixed}_degrees_${point}_x_${z_point}_z_${reflectivity}_ref_${cerenkov}_cer_${scintillation}_scint"
        cp sim_0.0_degrees_0.0_x.mac sim_${name}.mac
        sed -i 's;'"thetamin 0.0 deg"';'"thetamin ${fixed} deg"';g' sim_${name}.mac
        sed -i 's;'"thetamax 0.0 deg"';'"thetamax ${fixed} deg"';g' sim_${name}.mac
        sed -i 's;'"xmin 0.0 cm"';'"xmin ${point} cm"';g' sim_${name}.mac
        sed -i 's;'"xmax 0.0 cm"';'"xmax ${point} cm"';g' sim_${name}.mac
        # 0.1994 = sin(11.5 degrees), $point is the distance from 0.0, + is farther towards PMT, further back away from +z axis
        z_point=$(printf "%.1f" "$(bc -l <<< ${z_pos}-\($point*0.1994\))")
        #z_point=$(printf "%.1f" "$(bc -l <<< -11.0-\($point*0.1994\))")
    fi
    sed -i 's;'"fLGReflectivity 0.9"';'"fLGReflectivity ${reflectivity}"';g' sim_${name}.mac
    sed -i 's;'"fAllowCerenkov 1"';'"fAllowCerenkov ${cerenkov}"';g' sim_${name}.mac
    sed -i 's;'"fAllowScintillation 1"';'"fAllowScintillation ${scintillation}"';g' sim_${name}.mac
    sed -i 's;'"z -11.0 cm"';'"z ${z_point} cm"';g' sim_${name}.mac
    sed -i 's;'"0.0_degrees_0.0_x.root"';'"${name}.root"';g' sim_${name}.mac
    cd ../
    source /share/apps/root-5.34.36-build/bin/thisroot.sh
#    if [[ "$scintillation" != "1.0" ]] ; then
#        ./build/qsim-cerenkovOnly macros/sim_${name}.mac
#    elif [[ "$cerenkov" != "1.0" ]] ; then
#        ./build/qsim-scintOnly macros/sim_${name}.mac
#    else
        ./build/qsim macros/sim_${name}.mac
#    fi
    cd -
    source /share/apps/root-6.14.06-build/bin/thisroot.sh
    root -l -b -q get_pe.C'("'../qsimout_${name}.root'",'${reflectivity}','${cerenkov}','${scintillation}','${z_point}')'
    source /share/apps/root-5.34.36-build/bin/thisroot.sh
done
