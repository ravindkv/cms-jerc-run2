#ifndef GLOBALFLAG_H
#define GLOBALFLAG_H

#include <string>
#include <iostream>

class GlobalFlag {
public:
    // Enum classes for better organization
    enum class Year {
        NONE,
        Year2016Pre,
        Year2016Post,
        Year2017,
        Year2018
    };

    enum class Era {
        NONE,
     	Era2016PreBCD,
     	Era2016PreEF,
     	Era2016PostFGH,

     	Era2017B,
     	Era2017C,
     	Era2017D,
     	Era2017E,
     	Era2017F,

     	Era2018A,
     	Era2018B,
     	Era2018C,
     	Era2018D,
    };

    enum class Channel {
        NONE,
        GamJet,
        GamJetFake,
        ZeeJet,
        ZmmJet,
        MCTruth,
        Flavour,
        VetoMap,
        DiJet,
        IncJet,
        MultiJet,
        Wqqe,
        Wqqm
    };

    // Constructor and Destructor
    explicit GlobalFlag(std::string  outName);
    virtual ~GlobalFlag() = default;  // Virtual for proper cleanup in derived classes

    // Delete copy constructor and assignment operator to enforce Singleton (if needed)
    GlobalFlag(const GlobalFlag&) = delete;
    GlobalFlag& operator=(const GlobalFlag&) = delete;

    // Setter methods
    void setDebug(const bool& debug);
    void setNDebug(const int & nDebug);

    // Getter methods
    bool isDebug() const { return isDebug_; }
    int getNDebug() const { return nDebug_; }

    Year getYear() const { return year_; }
    Era getEra() const { return era_; }
    bool isData() const { return isData_; }
    bool isMC() const { return isMC_; }
    Channel getChannel() const { return channel_; }

    bool isQCD() const { return isQCD_; }
    bool isMG() const { return isMG_; }

    // New helper functions to return string representations
    std::string getChannelStr() const;
    std::string getYearStr() const;
    std::string getDataStr() const;
    std::string getMcStr() const;
    double getLumiPerYear() const;

    // Method to print active flags
    void printFlags() const;

private:
    // Private member variables
    std::string outName_;

    // Flags
    bool isDebug_ = false;
    int nDebug_ = 0;

    Year year_ = Year::NONE;
    Era  era_  = Era::NONE;
    bool isData_ = false;
    bool isMC_   = false;
    Channel channel_ = Channel::NONE;

    bool isQCD_ = false;
    bool isMG_ = false;

    // Helper methods
    void parseFlags();
};

#endif // GLOBALFLAG_H

