# cms-jerc-run2
Jet energy and resolution correction for Run3 and Phase 2

## Basic setups for EL9
* ssh rverma@lxplus9.cern.ch
* git clone git@github.com:ravindkv/cms-jerc-run2.git
* voms-proxy-init --voms cms --valid 24:00

Go to the Skim, Hist, or other directories and follow the README there

For Debugging (or catching the segmentation violation)

* Compile the code with flag -g in the Makefile (GCC=g++ -g -std=c++17)
* Run the code with gdp (
$ gdb --args ./runMain -o Data_2024Cv1_DiJet_JetMET1_Hist_1of100.root
* (gdb) run
* (gdb) backtrace
* (gdb) quit
