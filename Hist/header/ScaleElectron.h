#ifndef SCALEELECTRON_H
#define SCALEELECTRON_H

#include <unordered_map>
#include <memory>
#include <string>
#include "SkimTree.h"
#include "ScaleObject.h"
#include "TLorentzVector.h"

/**
 * @brief Class to apply Jet Energy Corrections (JEC) to jets in an event.
 * 
 * This class applies JECs up to a specified level. It is independent of channel-specific logic.
 */
class ScaleElectron {
public:

    ScaleElectron(ScaleObject *scaleObj, bool isData);
    
    void Initialize();
    void applyCorrections(std::shared_ptr<SkimTree>& skimT);

    // Getter functions to access the corrections maps
    const std::unordered_map<std::string, TLorentzVector>& getP4MapElectron1() const {
        return p4MapElectron1_;
    }
    void print() const;

private:
    ScaleObject *scaleObj_;
    bool isData_;

    // Jet correction maps
    std::unordered_map<std::string, TLorentzVector> p4MapElectron1_;

};

#endif // SCALEELECTRON_H

