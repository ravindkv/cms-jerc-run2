#ifndef APPLYJERC_H
#define APPLYJERC_H

#include <unordered_map>
#include <memory>
#include <string>
#include "SkimTree.h"
#include "ObjectScale.h"
#include "TLorentzVector.h"

/**
 * @brief Class to apply Jet Energy Corrections (JEC) to jets in an event.
 * 
 * This class applies JECs up to a specified level. It is independent of channel-specific logic.
 */
class ApplyJerc {
public:
    enum class CorrectionLevel {
        None = 0,
        L1Rc = 1,
        L2Rel = 2,
        L2L3Res = 3
    };

    ApplyJerc(ObjectScale *objScale, bool isData, bool applyJer);
    
    void Initialize();
    void applyCorrections(std::shared_ptr<SkimTree>& skimT, CorrectionLevel level);

    // Getter functions to access the corrections maps
    const std::unordered_map<std::string, TLorentzVector>& getP4MapJet1() const {
        return p4MapJet1_;
    }
    const std::unordered_map<std::string, TLorentzVector>& getP4MapJetSum() const {
        return p4MapJetSum_;
    }

    const std::unordered_map<std::string, TLorentzVector>& getP4MapMet() const {
        return p4MapMet_;
    }

    void print() const;

private:
    ObjectScale *objScale_;
    bool isData_;
    bool applyJer_;

    // Jet correction maps
    std::unordered_map<std::string, TLorentzVector> p4MapJet1_;
    std::unordered_map<std::string, TLorentzVector> p4MapJetSum_;

    // MET
    std::unordered_map<std::string, TLorentzVector> p4MapMet_;

    // Internal methods for correction steps
    void undoNanoAODCorrection(SkimTree& skimT, int i);
    void applyL1FastJetCorrection(SkimTree& skimT, int i);
    void applyL2RelativeCorrection(SkimTree& skimT, int i);
    void applyL2L3ResidualCorrection(SkimTree& skimT, int i);
    void applyJerCorrection(SkimTree& skimT, int i);
};

#endif // APPLYJERC_H
