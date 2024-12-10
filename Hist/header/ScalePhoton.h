#ifndef SCALEPHOTON_H
#define SCALEPHOTON_H

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
class ScalePhoton {
public:

    ScalePhoton(ScaleObject *scaleObj, bool isData);
    
    void Initialize();
    void applyCorrections(std::shared_ptr<SkimTree>& skimT);

    // Getter functions to access the corrections maps
    const std::unordered_map<std::string, TLorentzVector>& getP4MapPhoton1() const {
        return p4MapPhoton1_;
    }
    void print() const;

private:
    ScaleObject *scaleObj_;
    bool isData_;

    // Jet correction maps
    std::unordered_map<std::string, TLorentzVector> p4MapPhoton1_;

};

#endif // SCALEPHOTON_H

