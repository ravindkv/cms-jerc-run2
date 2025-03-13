# Inputs.py
from pathlib import Path

# Base directory for fit outputs
EOS_FIT_DIR = "/eos/cms/store/group/phys_jetmet/rverma/cms-jerc-run2/Fit"

# List of years to process
YEARS = [
    # '2016Pre',
    # '2016Post',
    '2017',
    # '2018'
]

# Channel configuration dictionary.
# For each channel, we define:
#  - "paths": a dict mapping a variation name to the corresponding histogram path in the ROOT file.
#             The "Base" key is mandatory.
#  - "scales": a dict mapping a scale variation name (e.g. "scale0.1") to a scaling factor.
CHANNELS_L3RES = {
    "Wqqm": {
        "paths": {
            "Base": "Base/passExactly2bJet/HistTTbar/p1HadWMassInAvgHadPt",
            "fsr": "fsr/passExactly2bJet/HistTTbar/p1HadWMassInAvgHadPt"
        },
        "scales": {
            "scale0.1": 0.1,
            "scale0.2": 0.2
        }
    },
    "Wqqe": {
        "paths": {
            "Base": "Base/passExactly2bJet/HistTTbar/p1HadWMassInAvgHadPt",
            "fsr": "fsr/passExactly2bJet/HistTTbar/p1HadWMassInAvgHadPt"
        },
        "scales": {
            "scale0.1": 0.1,
            "scale0.2": 0.2
        }
    }
}

