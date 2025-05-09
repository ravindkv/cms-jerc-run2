#include "GlobalFlag.h"

GlobalFlag::GlobalFlag(std::string  outName)
    : outName_(std::move(outName)), 
     
    nDebug_(100) 
    { 
    parseFlags();
}

void GlobalFlag::setDebug(const bool& debug){
    isDebug_ = debug;
}
void GlobalFlag::setNDebug(const int & nDebug){
    nDebug_ = nDebug;
}

std::string GlobalFlag::getChannelStr() const {
    switch(channel_) {
        case Channel::GamJetFake: return "GamJetFake";
        case Channel::GamJet:     return "GamJet";
        case Channel::ZeeJet:     return "ZeeJet";
        case Channel::ZmmJet:     return "ZmmJet";
        case Channel::MCTruth:    return "MCTruth";
        case Channel::Flavour:    return "Flavour";
        case Channel::VetoMap:    return "VetoMap";
        case Channel::DiJet:      return "DiJet";
        case Channel::IncJet:     return "IncJet";
        case Channel::MultiJet:   return "MultiJet";
        case Channel::Wqqe:       return "Wqqe";
        case Channel::Wqqm:       return "Wqqm";
        default:                  return "NONE";
    }
}

std::string GlobalFlag::getYearStr() const {
    switch(year_) {
        case Year::Year2016Pre:  return "2016Pre";
        case Year::Year2016Post: return "2016Post";
        case Year::Year2017:     return "2017";
        case Year::Year2018:     return "2018";
        default:                 return "NONE";
    }
}

double GlobalFlag::getLumiPerYear() const {
    switch(year_) {
        case Year::Year2016Pre:  return 19.5;
        case Year::Year2016Post: return 16.8;
        case Year::Year2017:     return 41.5;
        case Year::Year2018:     return 59.8;
        default:                 return 1.0;
    }
}


std::string GlobalFlag::getDataStr() const {
    return isData_ ? "Data" : "";
}

std::string GlobalFlag::getMcStr() const {
    return isMC_ ? "MC" : "";
}

void GlobalFlag::parseFlags() {
    // Parsing Year
    if (outName_.find("2016Pre") != std::string::npos) {
        year_ = Year::Year2016Pre;
    } else if (outName_.find("2016Post") != std::string::npos) {
        year_ = Year::Year2016Post;
    } else if (outName_.find("2017") != std::string::npos) {
        year_ = Year::Year2017;
    } else if (outName_.find("2018") != std::string::npos) {
        year_ = Year::Year2018;
    }

    // Parsing Era
    if (outName_.find("2016PreB") != std::string::npos ||
		outName_.find("2016PreC") != std::string::npos ||
		outName_.find("2016PreD") != std::string::npos) {
        era_ = Era::Era2016PreBCD;
    } else if (outName_.find("2016PreE") != std::string::npos||
		outName_.find("2016PreF") != std::string::npos) {
        era_ = Era::Era2016PreEF;

    } else if (outName_.find("2016PostF") != std::string::npos||
		outName_.find("2016PostG") != std::string::npos||
		outName_.find("2016PostH") != std::string::npos) {
        era_ = Era::Era2016PostFGH;

    } else if (outName_.find("2017B") != std::string::npos) {
        era_ = Era::Era2017B;
    } else if (outName_.find("2017C") != std::string::npos) {
        era_ = Era::Era2017C;
    } else if (outName_.find("2017D") != std::string::npos) {
        era_ = Era::Era2017D;
    } else if (outName_.find("2017E") != std::string::npos) {
        era_ = Era::Era2017E;
    } else if (outName_.find("2017F") != std::string::npos) {
        era_ = Era::Era2017F;

    } else if (outName_.find("2018A") != std::string::npos) {
        era_ = Era::Era2018A;
    } else if (outName_.find("2018B") != std::string::npos) {
        era_ = Era::Era2018B;
    } else if (outName_.find("2018C") != std::string::npos) {
        era_ = Era::Era2018C;
    } else if (outName_.find("2018D") != std::string::npos) {
        era_ = Era::Era2018D;
	}

    // Parsing Data or MC
    if (outName_.find("Data") != std::string::npos) {
        isData_ = true;
    }
    if (outName_.find("MC") != std::string::npos) {
        isMC_ = true;
    }

    // Parsing Channels
    if (outName_.find("GamJetFake") != std::string::npos) {
        channel_ = Channel::GamJetFake;
    }else if (outName_.find("GamJet") != std::string::npos) {
        channel_ = Channel::GamJet;
    } else if (outName_.find("ZeeJet") != std::string::npos) {
        channel_ = Channel::ZeeJet;
    } else if (outName_.find("ZmmJet") != std::string::npos) {
        channel_ = Channel::ZmmJet;
    } else if (outName_.find("MCTruth") != std::string::npos) {
        channel_ = Channel::MCTruth;
    } else if (outName_.find("Flavour") != std::string::npos) {
        channel_ = Channel::Flavour;
    } else if (outName_.find("VetoMap") != std::string::npos) {
        channel_ = Channel::VetoMap;
    } else if (outName_.find("DiJet") != std::string::npos) {
        channel_ = Channel::DiJet;
    } else if (outName_.find("IncJet") != std::string::npos) {
        channel_ = Channel::IncJet;
    } else if (outName_.find("MultiJet") != std::string::npos) {
        channel_ = Channel::MultiJet;
    } else if (outName_.find("Wqqe") != std::string::npos) {
        channel_ = Channel::Wqqe;
    } else if (outName_.find("Wqqm") != std::string::npos) {
        channel_ = Channel::Wqqm;
    }

    // Parsing Samples
    if (outName_.find("QCD") != std::string::npos) {
        isQCD_ = true;
    }
    if (outName_.find("MG") != std::string::npos) { // Assuming "MG" refers to MadGraph
        isMG_ = true;
    }
}

void GlobalFlag::printFlags() const {
    if (isDebug_){
        std::cout << "isDebug_ = true" << '\n';
        std::cout << "nDebug_ = " << nDebug_ << '\n';
    }

    // Print Year
    switch (year_) {
        case Year::Year2016Pre:
            std::cout << "Year = 2016Pre" << '\n';
            break;
        case Year::Year2016Post:
            std::cout << "Year = 2016Post" << '\n';
            break;
        case Year::Year2017:
            std::cout << "Year = 2017" << '\n';
            break;
        case Year::Year2018:
            std::cout << "Year = 2018" << '\n';
            break;
        default:
            std::cout << "Year = NONE" << '\n';
    }

    // Print Era
    switch (era_) {
        case Era::Era2016PreBCD:
            std::cout << "Era = 2016PreBCD" << '\n';
            break;
        case Era::Era2016PreEF:
            std::cout << "Era = 2016PreEF" << '\n';
            break;

        case Era::Era2016PostFGH:
            std::cout << "Era = 2016PostFGH" << '\n';
            break;

        case Era::Era2017B:
            std::cout << "Era = 2017B" << '\n';
            break;
        case Era::Era2017C:
            std::cout << "Era = 2017C" << '\n';
            break;
        case Era::Era2017D:
            std::cout << "Era = 2017D" << '\n';
            break;
        case Era::Era2017E:
            std::cout << "Era = 2017E" << '\n';
            break;
        case Era::Era2017F:
            std::cout << "Era = 2017F" << '\n';
            break;

        case Era::Era2018A:
            std::cout << "Era = 2018A" << '\n';
            break;
        case Era::Era2018B:
            std::cout << "Era = 2018B" << '\n';
            break;
        case Era::Era2018C:
            std::cout << "Era = 2018C" << '\n';
            break;
        case Era::Era2018D:
            std::cout << "Era = 2018D" << '\n';
            break;
        default:
            std::cout << "Era = NONE" << '\n';
    }

    // Print Channel
    switch (channel_) {
        case Channel::GamJetFake:
            std::cout << "Channel = GamJetFake" << '\n';
            break;
        case Channel::GamJet:
            std::cout << "Channel = GamJet" << '\n';
            break;
        case Channel::ZeeJet:
            std::cout << "Channel = ZeeJet" << '\n';
            break;
        case Channel::ZmmJet:
            std::cout << "Channel = ZmmJet" << '\n';
            break;
        case Channel::MCTruth:
            std::cout << "Channel = MCTruth" << '\n';
            break;
        case Channel::Flavour:
            std::cout << "Channel = Flavour" << '\n';
            break;
        case Channel::VetoMap:
            std::cout << "Channel = VetoMap" << '\n';
            break;
        case Channel::DiJet:
            std::cout << "Channel = DiJet" << '\n';
            break;
        case Channel::IncJet:
            std::cout << "Channel = IncJet" << '\n';
            break;
        case Channel::MultiJet:
            std::cout << "Channel = MultiJet" << '\n';
            break;
        case Channel::Wqqe:
            std::cout << "Channel = Wqqe" << '\n';
            break;
        case Channel::Wqqm:
            std::cout << "Channel = Wqqm" << '\n';
            break;
        default:
            std::cout << "Channel = NONE" << '\n';
    }

    // Print Data or MC
    if(isData_) std::cout << "isData = true" << '\n'; 
    if(isMC_) std::cout << "isMC = true" << '\n'; 
    // Print Samples
    if(isQCD_) std::cout << "isQCD = true" << '\n'; 
    if(isMG_) std::cout << "isMG = true" << '\n'; 

}

