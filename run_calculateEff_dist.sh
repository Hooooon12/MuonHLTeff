#!/bin/bash

if [ -z $CMSSW_VERSION ]
then
    echo No CMSSW. Exiting...
    exit 1
fi

runningDir=`pwd`
RUNSCRIPT=$(realpath calculateEff_dist.C)

INPUT=$1
OUTPUT=$2
SAMPLE=$3
nIter=$4

SCRIPT=condor_calculateEff_dist_${nIter}.sh
echo "#!/bin/bash" > $SCRIPT

if [[ $HOSTNAME == *"knu"* ]]
then
    echo "export LD_LIBRARY_PATH=\"\$LD_LIBRARY_PATH:/usr/lib64/dcap\"" >> $SCRIPT
    echo "echo KNU here!" >> $SCRIPT
    echo "echo LD_LIBRARY_PATH : \$LD_LIBRARY_PATH" >> $SCRIPT
fi
echo cd $runningDir >> $SCRIPT
echo "echo I\'m here:" >> $SCRIPT
echo pwd >> $SCRIPT
echo echo \$CMSSW_VERSION >> $SCRIPT
echo echo Now running... >> $SCRIPT
echo 'echo -e ".L '$RUNSCRIPT'\n calculateEff_dist('\\\"${INPUT}\\\"','\\\"${OUTPUT}\\\"');\n .q"|root -l -b' >> $SCRIPT
chmod +x $SCRIPT

condor_submit -batch-name calculateEff_dist_${SAMPLE} << EOF
executable = $SCRIPT
universe = vanilla
log = condor_calculateEff_dist_${nIter}_log.txt
output = condor_calculateEff_dist_${nIter}_out.txt
error = condor_calculateEff_dist_${nIter}_error.txt
getenv = True
#request_cpus = 10
#request_memory = 8G
#request_memory = 30G
accounting_group = group_cms
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
queue
EOF
