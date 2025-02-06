#!/usr/bin/env python3
import os
import glob
import sys
import ROOT

# Run in batch mode so no graphics windows appear.
ROOT.gROOT.SetBatch(True)

sys.dont_write_bytecode = True
sys.path.insert(0, os.getcwd().replace("condor/merge", ""))

from Inputs import eosHistDir, Years

def get_hist_events(file_path, hist_path):
    """
    Opens a ROOT file, retrieves the histogram at `hist_path`
    and returns its integral (i.e. the total event count).
    """
    f = ROOT.TFile.Open(file_path)
    if not f or f.IsZombie():
        print(f"[ERROR] Could not open file: {file_path}")
        return 0

    hist = f.Get(hist_path)
    if not hist:
        print(f"[ERROR] Histogram '{hist_path}' not found in file: {file_path}")
        f.Close()
        return 0

    events = hist.Integral()
    f.Close()
    return events

def get_total_events(file_pattern, hist_path):
    """
    Finds all files matching file_pattern, retrieves the histogram from each file,
    and returns the sum of the event counts.
    """
    files = glob.glob(file_pattern)
    if not files:
        print(f"[WARNING] No files found for pattern: {file_pattern}")
        return 0

    total = 0
    for fpath in files:
        ev = get_hist_events(fpath, hist_path)
        print(f"File: {fpath}\n  --> Events: {ev}")
        total += ev
    return total

def main():
    # Configuration variables
    histPathInFile = "Base/passExactly1Ref/HistRef/h1EventInRefPt"
    channel = "GamJetFake"
    baseDir = f"{eosHistDir}/{channel}"
    #sampName = "DYJetsToLLM50Herwig"
    sampName = "QCD"
    
    # We'll compute totals for three categories by summing over individual years:
    total_separate = 0
    total_mergedJobs = 0
    total_mergedEras = 0

    print(f"Validating event counts for sample: {sampName}\n")
    
    # Loop over each year and sum the counts.
    for year in Years:
        print(f"\n--- Processing year: {year} ---")
        
        # Pattern for individual (separate) files.
        sep_pattern = os.path.join(baseDir, year, f"*{sampName}*")
        events_sep = get_total_events(sep_pattern, histPathInFile)
        print(f"Total events in separate files for {year}: {events_sep}")
        total_separate += events_sep

        # Pattern for merged jobs files.
        mj_pattern = os.path.join(baseDir, "MergedJobs", year, f"*{sampName}*")
        events_mj = get_total_events(mj_pattern, histPathInFile)
        print(f"Total events in MergedJobs for {year}: {events_mj}")
        total_mergedJobs += events_mj

        # Pattern for merged eras files.
        me_pattern = os.path.join(baseDir, "MergedEras", year, f"*{sampName}*")
        events_me = get_total_events(me_pattern, histPathInFile)
        print(f"Total events in MergedEras for {year}: {events_me}")
        total_mergedEras += events_me

    # Now get the total events from the merged Run2 file (which is after merging the years)
    mergedYears_pattern = os.path.join(baseDir, "MergedYears", "Run2", f"*{sampName}*")
    total_mergedYears = get_total_events(mergedYears_pattern, histPathInFile)
    print(f"Total events in MergedYears/Run2 (combined years): {total_mergedYears}\n")

    # Summarize results
    results = {
        "Separate (Run2)": int(total_separate),
        "MergedJobs (Run2)"   : int(total_mergedJobs),
        "MergedEras (Run2)"   : int(total_mergedEras),
        "MergedYears/Run2"         : int(total_mergedYears),
    }
    
    print("==== Summary of Event Counts ====")
    for label, total in results.items():
        print(f"{label:25s}: {total}")
    
    # Compare the totals. If the merging is done properly all categories should yield the same number.
    unique_counts = set(results.values())
    if len(unique_counts) == 1:
        print("\n[PASS] All event counts match!")
    else:
        print("\n[FAIL] Mismatch in event counts detected!")
        for label, total in results.items():
            print(f"  {label}: {total}")

if __name__ == "__main__":
    main()

