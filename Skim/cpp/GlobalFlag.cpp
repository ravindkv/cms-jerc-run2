#include "GlobalFlag.h"
#include <stdexcept> // for std::runtime_error

GlobalFlag::GlobalFlag(const std::string& outName) : 
    isDebug(false), 
    is2016Pre(false), 
    is2016Post(false), 
    is2017(false), 
    is2018(false), 
    isData(false), 
    isMC(false), 
    isZeeJet(false), 
    isZmmJet(false), 
    isGamJet(false), 
    isGamJetFake(false), 
    isMultiJet(false), 
    isDiJet(false), 
    isWqqe(false), 
    isWqqm(false), 
    isQCD(false), 
    isMG(false)
{
    // --- Year ---
    const std::string year2016PreStr  = "2016Pre";
    const std::string year2016PostStr = "2016Post";
    const std::string year2017Str     = "2017";
    const std::string year2018Str     = "2018";

    int yearCount = 0;
    if (outName.find(year2016PreStr) != std::string::npos) {
        is2016Pre = true;
        yearStr = year2016PreStr;
        ++yearCount;
    }
    if (outName.find(year2016PostStr) != std::string::npos) {
        is2016Post = true;
        yearStr = year2016PostStr;
        ++yearCount;
    }
    if (outName.find(year2017Str) != std::string::npos) {
        is2017 = true;
        yearStr = year2017Str;
        ++yearCount;
    }
    if (outName.find(year2018Str) != std::string::npos) {
        is2018 = true;
        yearStr = year2018Str;
        ++yearCount;
    }
    if (yearCount != 1) {
        throw std::runtime_error("Error: Exactly one year flag must be set in " + outName);
    }

    // --- Data or MC ---
    const std::string dataStr = "Data";
    const std::string mcStr   = "MC";

    bool dataFound = (outName.find(dataStr) != std::string::npos);
    bool mcFound   = (outName.find(mcStr) != std::string::npos);
    if (dataFound && mcFound) {
        throw std::runtime_error("Error: Both Data and MC flags are set in " + outName);
    }
    else if (!dataFound && !mcFound) {
        throw std::runtime_error("Error: Neither Data nor MC flag is set in " + outName);
    }
    else if (dataFound) {
        isData = true;
    }
    else { // mcFound
        isMC = true;
    }

    // --- Channels ---
    const std::string channelZeeJet      = "ZeeJet";
    const std::string channelZmmJet      = "ZmmJet";
    const std::string channelGamJetFake  = "GamJetFake";
    const std::string channelGamJet      = "GamJet";
    const std::string channelMultiJet    = "MultiJet";
    const std::string channelDiJet       = "DiJet";
    const std::string channelWqqe        = "Wqqe";
    const std::string channelWqqm        = "Wqqm";

    int channelCount = 0;
    if (outName.find(channelZeeJet) != std::string::npos) {
        isZeeJet = true;
        channelStr = channelZeeJet;
        ++channelCount;
    }
    if (outName.find(channelZmmJet) != std::string::npos) {
        isZmmJet = true;
        channelStr = channelZmmJet;
        ++channelCount;
    }
    if (outName.find(channelGamJetFake) != std::string::npos) {
        isGamJetFake = true;
        channelStr = channelGamJetFake;
        ++channelCount;
    }
    // If GamJetFake was found, this one should not be set.
    else if (outName.find(channelGamJet) != std::string::npos) {
        isGamJet = true;
        channelStr = channelGamJet;
        ++channelCount;
    }
    if (outName.find(channelMultiJet) != std::string::npos) {
        isMultiJet = true;
        channelStr = channelMultiJet;
        ++channelCount;
    }
    if (outName.find(channelDiJet) != std::string::npos) {
        isDiJet = true;
        channelStr = channelDiJet;
        ++channelCount;
    }
    if (outName.find(channelWqqe) != std::string::npos) {
        isWqqe = true;
        channelStr = channelWqqe;
        ++channelCount;
    }
    if (outName.find(channelWqqm) != std::string::npos) {
        isWqqm = true;
        channelStr = channelWqqm;
        ++channelCount;
    }
    if (channelCount != 1) {
        throw std::runtime_error("Error: Exactly one channel flag must be set in " + outName);
    }

    // --- Sample (e.g. QCD or MG) ---
    if (outName.find("QCD") != std::string::npos) isQCD = true;
    if (outName.find("MC") != std::string::npos)  isMG  = true;
}

void GlobalFlag::setDebug(bool debug) {
    isDebug = debug;
}

void GlobalFlag::printFlag(){
    if (isDebug   ) std::cout << "isDebug   = true" << '\n';
    if (is2016Pre ) std::cout << "is2016Pre = true" << '\n';
    if (is2016Post) std::cout << "is2016Post= true" << '\n';
    if (is2017    ) std::cout << "is2017    = true" << '\n';
    if (is2018    ) std::cout << "is2018    = true" << '\n';
    if (isData    ) std::cout << "isData    = true" << '\n';
    if (isMC      ) std::cout << "isMC      = true" << '\n';
    if (isZeeJet  ) std::cout << "isZeeJet  = true" << '\n';
    if (isZmmJet  ) std::cout << "isZmmJet  = true" << '\n';
    if (isGamJet  ) std::cout << "isGamJet  = true" << '\n';
    if (isGamJetFake) std::cout << "isGamJetFake = true" << '\n';
    if (isMultiJet) std::cout << "isMultiJet = true" << '\n';
    if (isDiJet   ) std::cout << "isDiJet   = true" << '\n';
    if (isWqqe    ) std::cout << "isWqqe    = true" << '\n';
    if (isWqqm    ) std::cout << "isWqqm    = true" << '\n';
    if (isQCD     ) std::cout << "isQCD     = true" << '\n';
    if (isMG      ) std::cout << "isMG      = true" << '\n';
}

