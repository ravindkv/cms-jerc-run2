# Config.py

inputShapes = {
    "Resp": {},
    "chf": {},
    "nhf": {},
    "nef": {}
}
# Populating inputShapes 
inputShapes["Resp"].update({
    "ftd": {
        "ispositive": 0,
        "type": "ftd",
        "form": "-0.116-0.6417*pow(x/208.,-0.3051)+23.63/x"
    },
    "fp": {
        "ispositive": 0,
        "type": "fp",
        "form": "-0.8295"
    },
    "fhx": {
        "ispositive": 0,
        "type": "fhx",
        "form": "0.8904+1.082*pow(x/1408,1.204)/(1+pow(x/1408,1.204))*(1-pow(x/1408,-1.204))"
    },
    "fhh": {
        "ispositive": 0,
        "type": "fhh",
        "form": "-0.7938-0.5798*pow(x/396.1,1.412)/(1+pow(x/396.1,1.412))*(1-pow(x/396.1,-1.412))"
    },
    "feh": {
        "ispositive": 0,
        "type": "feh",
        "form": "-0.2603-0.2196*pow(x/409.4,1.276)/(1+pow(x/409.4,1.276))*(1-pow(x/409.4,-1.276))"
    },
    "fhw": {
        "ispositive": 0,
        "type": "fhw",
        "form": "0.3*(0.9526-0.3883*(1+(pow(x/1285,2.46)-1)/(pow(x/1285,2.46)+1))+18.1/x-2.062*log(x)/x)"
    },
    "fl1-1": {
        "ispositive": 0,
        "type": "fl1",
        "form": "(1-(0.350077+0.553560*log(x)-0.0527681*pow(log(x),2))/x-1)"
    },
    "ftd-ftm": {
        "ispositive": 0,
        "type": "ftd-ftm",
        "form": "3*((-0.116-0.6417*pow(x/208.,-0.3051)+23.63/x)-(0.2683-0.6994*pow(x/208.,-0.3051)+18.49/x))"
    },
    "f1q3-1": {
        "ispositive": 0,
        "type": "f1q3-1",
        "form": "0.01*(0.7966+0.9311*(pow(0.01*x,-1)-1))"
    }
})

inputShapes["chf"].update({
    "ftmg_chf": {
        "ispositive": 0,
        "type": "ftd",
        "form": "1.982-2.678*(1+(pow(x/47.02,0.262)-1)/(pow(x/47.02,0.262)+1))+0.1494*pow(x,+0.3)-3.097/x"
    },
    "fp_chf": {
        "ispositive": 0,
        "type": "fp",
        "form": "0.3333+0.7433*(1+(pow(x/1023,0.3926)-1)/(pow(x/1023,0.3926)+1))-0.09446*pow(x,0.2883)"
    },
    "fhx_chf": {
        "ispositive": 0,
        "type": "fhx",
        "form": "-0.0637-0.2811*(1+(pow(x/4531,-0.3172)-1)/(pow(x/4531,-0.3172)+1))+1.071*pow(x,-0.153)"
    },
    "fhw_chf": {
        "ispositive": 0,
        "type": "fhw",
        "form": "-0.2176+1.064e-05*pow(x,1.373)+0/x"
    }
})

inputShapes["nhf"].update({
    "ftmg_nhf": {
        "ispositive": 0,
        "type": "ftd",
        "form": "-0.01022-0.1962*(1+(pow(x/4000,3.071)-1)/(pow(x/4000,3.071)+1))+0.04211*pow(x,+0.3)+0.01005/x"
    },
    "fp_nhf": {
        "ispositive": 0,
        "type": "fp",
        "form": "0.07395+0*(1+(pow(x/1000,258.2)-1)/(pow(x/1000,258.2)+1))+1.223e-05*pow(x,1.158)"
    },
    "fhx_nhf": {
        "ispositive": 0,
        "type": "fhx",
        "form": "-0.295+0.09444*(1+(pow(x/2713,0.06437)-1)/(pow(x/2713,0.06437)+1))+[p4]*pow(x,0.2845)"
    },
    "fhw_nhf": {
        "ispositive": 0,
        "type": "fhw",
        "form": "-5.151+4.495*pow(x,0.03335)-12.3/x"
    }
})

inputShapes["nef"].update({
    "ftmg_nef": {
        "ispositive": 0,
        "type": "ftd",
        "form": "0.07453+0.1457*(1+(pow(x/1131,-3.68)-1)/(pow(x/1131,-3.68)+1))-0.4155*pow(x,-0.3)-1.878/x"
    },
    "fp_nef": {
        "ispositive": 0,
        "type": "fp",
        "form": "2.283+0*(1+(pow(x/1000,1.302)-1)/(pow(x/1000,1.302)+1))-2.738*pow(x,0.002452)"
    },
    "fhx_nef": {
        "ispositive": 0,
        "type": "fhx",
        "form": "0.05474-0.003141*(1+(pow(x/798.6,78.84)-1)/(pow(x/798.6,78.84)+1))-0.000957*pow(x,0.76)"
    },
    "fhw_nef": {
        "ispositive": 0,
        "type": "fhw",
        "form": "0.8417-0.2605*pow(x,0.2289)+2.426/x"
    }
})

def base_fit_config(input_var):
    """
    Returns the base configuration template for the fit.
    """
    if input_var not in inputShapes:
        raise ValueError(f"Invalid input variable: {input_var}. Choose from {list(inputShapes.keys())}")

    return {
        "input_base": input_var,
        "input_syst": ["herr"],  # default systematic; may be extended
        "input_shape": inputShapes[input_var],
        "input_norm": ["norm"]
    }

def update_fit_config_for_channel_year(channel, year, input_var="Resp", extra_systs=None):
    """
    Update the base configuration for a specific channel and year.
    Optionally include extra systematic variations (list of str) in the config.
    """
    config = base_fit_config(input_var)
    
    # Example override for channel 'Wqqe' in 2017
    if channel == "Wqqe" and year == "2017":
        if "ftd" in config["input_shape"]:
            config["input_shape"]["ftd"]["form"] = "-0.200-0.600*pow(x/210.,-0.300)+20.0/x"
        if "fp" in config["input_shape"]:
            config["input_shape"]["fp"]["form"] = "-0.900"
    
    if extra_systs:
        # Append extra systematic variations to input_syst, avoiding duplicates.
        for syst in extra_systs:
            if syst not in config["input_syst"]:
                config["input_syst"].append(syst)
    
    return config
