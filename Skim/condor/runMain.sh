#!/bin/bash
#To be run on remote machine
#Take input arguments as an array
myArray=( "$@" )
#Array: Size=$#, an element=$1, all element = $@

printf "Start skimming at ";/bin/date
printf "Worker node hostname ";/bin/hostname

if [ -z ${_CONDOR_SCRATCH_DIR} ] ; then 
    echo "Running Interactively" ; 
else
    echo "Running In Batch"
    echo ${_CONDOR_SCRATCH_DIR}
    export SCRAM_ARCH=el9_amd64_gcc10
    scramv1 project CMSSW CMSSW_13_3_0
    cd CMSSW_13_3_0/src
    eval `scramv1 runtime -sh`
    cd ../..
	tar --strip-components=1 -zxf Skim.tar.gz
fi

#Run for Base, Signal region
echo "All arguements: "$@
echo "Number of arguements: "$#
oName=$1
outDir=$2
echo "./runMain -o oName"
./runMain -o ${oName}

printf "Done skimming at ";/bin/date
#---------------------------------------------
#Copy the ouput root files
#---------------------------------------------
if [ -z ${_CONDOR_SCRATCH_DIR} ] ; then
    echo "Running Interactively" ;
else
    xrdcp -f output/${oName} ${outDir}
    echo "Cleanup"
    rm -rf CMSSW*
    rm output/*.root 
fi
printf "Done ";/bin/date
