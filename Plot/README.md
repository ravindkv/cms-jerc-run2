## Produce  plots from Hists 

### Step-1: get list of Hist files
* cd input
* python3 getFiles.py  

Merged all jsons in to one master file
* python3 mergeJson.py

### Step-2: produce plot 
* cd ..
* make
* ./runMain -h

then run the commands displayed on the screen


### Step-3: compile latex

* cd output
* pdflatex Channel_Plot.tex
