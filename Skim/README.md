## Produce skim file from NanoAOD

### Step-1: get input root files of samples from DAS
* cd input
* python3 getFiles.py  

Have a look at the files produced from the above command

### Step-2: produce ONE skim file from nanofiles 
* cd ..
* make
* ./runMain -h
Run any of the commands displayed on the screen

### Step-3: submit condor jobs to produce MANY skims 

* cd condor
* python createJobFiles.py
* cd tmpSub

It is good to submit few jobs first and then
* source submitAll.sh

Monitor the condor jobs on linux terminal
* condor_q 
* condor_tail 7217104.0
* condor_q -better-analyze 75671743.5

Once condor_q is DONE. Check the finished jobs
* cd ..
* python3 checkFinishedJobs.py 

This will open each file and perform many checks. It  will  create job files for the failed 
jobs which can be resubmitted
* cd tmpSub
* condor_submit resubJobs.jdl

Once condor_q is DONE. Check the finished jobs again
* cd ..
* python3 checkFinishedJobs.py
and follow the above steps untill all jobs are finished

