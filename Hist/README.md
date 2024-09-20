## Produce Histos file from Skim

### Step-1: get list of Skim files
* cd input
* python3 getFiles.py  

Have a look at the files produced from the above command

### Step-2: produce ONE hist file from skim files 
* cd ..
* make
* ./makeHist -h

or 
* ./makeHist

### Step-3: submit condor jobs to produce MANY hist files 

* cd condor
* python createJobsFiles.py
* cd tmpSub
* source submitAll.sh

Monitor the condor jobs on linux terminal
* condor_q 
* condor_q -better-analyze 75671743.5

Once condor_q is DONE. Check the finished jobs
* cd ..
* python3 checkJobStatus.py

This will open each file and perform many checks. It  will  create JDL files for the failed 
jobs which can be resubmitted
* cd tmpSub
* condor_submit resubJobs.jdl


Once ALL jobs are finished, merge them:
* python mergeJobs.py
