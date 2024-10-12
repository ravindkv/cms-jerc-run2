#include "GlobalFlag.h"

GlobalFlag::GlobalFlag(const std::string& outName)
    : outName_(outName), 
    isDebug_(false), 
    nDebug_(100),
    isData_(false), 
    isMC_(false), 
    isQCD_(false), 
    isMG_(false){ 
    parseFlags();
}

void GlobalFlag::setDebug(const bool& debug){
    isDebug_ = debug;
}
void GlobalFlag::setNDebug(const int & nDebug){
    nDebug_ = nDebug;
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
    if (outName_.find("GamJet") != std::string::npos) {
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
    } else if (outName_.find("Wqq") != std::string::npos) {
        channel_ = Channel::Wqq;
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
        std::cout << "isDebug_ = true" << std::endl;
        std::cout << "nDebug_ = " << nDebug_ << std::endl;
    }

    // Print Year
    switch (year_) {
        case Year::Year2016Pre:
            std::cout << "Year = 2016Pre" << std::endl;
            break;
        case Year::Year2016Post:
            std::cout << "Year = 2016Post" << std::endl;
            break;
        case Year::Year2017:
            std::cout << "Year = 2017" << std::endl;
            break;
        case Year::Year2018:
            std::cout << "Year = 2018" << std::endl;
            break;
        default:
            std::cout << "Year = NONE" << std::endl;
    }

    // Print Era
    switch (era_) {
        case Era::Era2016PreBCD:
            std::cout << "Era = 2016PreBCD" << std::endl;
            break;
        case Era::Era2016PreEF:
            std::cout << "Era = 2016PreEF" << std::endl;
            break;

        case Era::Era2016PostFGH:
            std::cout << "Era = 2016PostFGH" << std::endl;
            break;

        case Era::Era2017B:
            std::cout << "Era = 2017B" << std::endl;
            break;
        case Era::Era2017C:
            std::cout << "Era = 2017C" << std::endl;
            break;
        case Era::Era2017D:
            std::cout << "Era = 2017D" << std::endl;
            break;
        case Era::Era2017E:
            std::cout << "Era = 2017E" << std::endl;
            break;
        case Era::Era2017F:
            std::cout << "Era = 2017F" << std::endl;
            break;

        case Era::Era2018A:
            std::cout << "Era = 2018A" << std::endl;
            break;
        case Era::Era2018B:
            std::cout << "Era = 2018B" << std::endl;
            break;
        case Era::Era2018C:
            std::cout << "Era = 2018C" << std::endl;
            break;
        case Era::Era2018D:
            std::cout << "Era = 2018D" << std::endl;
            break;
        default:
            std::cout << "Era = NONE" << std::endl;
    }

    // Print Channel
    switch (channel_) {
        case Channel::GamJet:
            std::cout << "Channel = GamJet" << std::endl;
            break;
        case Channel::ZeeJet:
            std::cout << "Channel = ZeeJet" << std::endl;
            break;
        case Channel::ZmmJet:
            std::cout << "Channel = ZmmJet" << std::endl;
            break;
        case Channel::MCTruth:
            std::cout << "Channel = MCTruth" << std::endl;
            break;
        case Channel::Flavour:
            std::cout << "Channel = Flavour" << std::endl;
            break;
        case Channel::VetoMap:
            std::cout << "Channel = VetoMap" << std::endl;
            break;
        case Channel::DiJet:
            std::cout << "Channel = DiJet" << std::endl;
            break;
        case Channel::IncJet:
            std::cout << "Channel = IncJet" << std::endl;
            break;
        case Channel::MultiJet:
            std::cout << "Channel = MultiJet" << std::endl;
            break;
        case Channel::Wqq:
            std::cout << "Channel = Wqq" << std::endl;
            break;
        default:
            std::cout << "Channel = NONE" << std::endl;
    }

    // Print Data or MC
    if(isData_) std::cout << "isData = true" << std::endl; 
    if(isMC_) std::cout << "isMC = true" << std::endl; 
    // Print Samples
    if(isQCD_) std::cout << "isQCD = true" << std::endl; 
    if(isMG_) std::cout << "isMG = true" << std::endl; 

}

