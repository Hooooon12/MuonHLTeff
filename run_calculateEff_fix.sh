#!/bin/bash

if [ -z $CMSSW_VERSION ]
then
    echo No CMSSW. Exiting...
    exit 1
fi

runningDir=`pwd`
RUNSCRIPT=$(realpath calculateEff_fix.C)

while read -r line;
    do
        MYLIST=();
        for word in $line;
            do
#                echo $word;
                MYLIST+=($word);
            done;

        ####Test code start####
        SCRIPT=condor_calculateEff_fix_${MYLIST[2]}.sh
        echo "#!/bin/bash" > $SCRIPT
        
        echo cd $runningDir >> $SCRIPT
        echo "echo I\'m here:" >> $SCRIPT
        echo pwd >> $SCRIPT
        echo echo \$CMSSW_VERSION >> $SCRIPT
        echo echo Now running... >> $SCRIPT
        echo 'echo -e ".L '$RUNSCRIPT'\n calculateEff_fix('${MYLIST[0]}','${MYLIST[1]}');\n .q"|root -l -b' >> $SCRIPT
        chmod +x $SCRIPT
        
        condor_submit -batch-name calculateEff_fix << EOF
        executable = $SCRIPT
        universe = vanilla
        log = condor_calculateEff_fix_${MYLIST[2]}_log.txt
        output = condor_calculateEff_fix_${MYLIST[2]}_out.txt
        error = condor_calculateEff_fix_${MYLIST[2]}_error.txt
        getenv = True
        request_cpus = 10
        request_memory = 8G
        accounting_group = group_cms
        should_transfer_files = YES
        when_to_transfer_output = ON_EXIT
        queue
EOF
        #echo ${MYLIST[1]};
    done < list_input_fix.txt; 


#SCRIPT=condor_calculateEff_fix.sh
#echo "#!/bin/bash" > $SCRIPT
#
#echo cd $runningDir >> $SCRIPT
#echo "echo I\'m here:" >> $SCRIPT
#echo pwd >> $SCRIPT
#echo echo \$CMSSW_VERSION >> $SCRIPT
#echo echo Now running... >> $SCRIPT
#echo 'echo -e ".L '$RUNSCRIPT'+\n calculateEff_fix(${MYLIST[0]},${MYLIST[1]});\n .q"|root -l -b' >> $SCRIPT
#chmod +x $SCRIPT
#
#condor_submit -batch-name calculateEff_fix << EOF
#executable = $SCRIPT
#universe = vanilla
#log = condor_calculateEff_fix_log.txt
#output = condor_calculateEff_fix_out.txt
#error = condor_calculateEff_fix_error.txt
#getenv = True
#request_cpus = 10
#request_memory = 8G
#accounting_group = group_cms
#should_transfer_files = YES
#when_to_transfer_output = ON_EXIT
#queue
#EOF
#
