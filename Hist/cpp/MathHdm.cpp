#include "MathHdm.h"
#include "TMath.h"  // For TMath::Pi() if needed

// Constructor implementation
MathHdm::MathHdm(const GlobalFlag& globalFlags)
    :globalFlags_(globalFlags),
    year_(globalFlags_.getYear()),
    channel_(globalFlags_.getChannel()),
    isDebug_(globalFlags_.isDebug()){
}
//---------------------------------------------------------------------
// compute:
//   Computes derived 4-vectors and scalar variables based on the inputs.
//---------------------------------------------------------------------
void MathHdm::calcResponse(const TLorentzVector& p4CorrMet,
                        const TLorentzVector& p4Ref,
                        const TLorentzVector& p4Jet1,
                        const TLorentzVector& p4Jetn)
{
    p4CorrMet_ = p4CorrMet;
    p4Ref_     = p4Ref;
    p4Jet1_    = p4Jet1;
    p4Jetn_    = p4Jetn;

    p4Conservation_ = p4CorrMet -(p4Ref + p4Jet1 + p4Jetn);

    // Calculate intermediate 4-vectors according to the provided formulas.
    // Note: p4Ref.Pt() is used for the pt reference.
    double ptRef = p4Ref_.Pt();
    double ptRefSqr = ptRef*ptRef;
    if (ptRef == 0) {
        std::cerr << "Error in MathHdm::compute: p4Ref Pt is zero." << std::endl;
        return;
    }
    
    //Eq.8 of hdm_2023082.pdf
    // p4Metu = p4CorrMet + p4Ref + p4Jet1 + p4Jetn;
    p4Metu_ = p4CorrMet_ + p4Ref_ + p4Jet1_ + p4Jetn_;

    // p4Met1 = - p4Ref - p4Jet1 ;
    p4Met1_ = -p4Ref_ - p4Jet1_;

    // p4Metn = - p4Jetn;
    p4Metn_ = -p4Jetn_;

    // p4Metnu = p4Metn + 1.1 * p4Metu;
    p4Metnu_ = p4Metn_ + 1.1 * p4Metu_;

    // Make 4-vectors transverse (set Pz to zero)
    makeTransverse(p4CorrMet_);
    makeTransverse(p4Met1_);
    makeTransverse(p4Metn_);
    makeTransverse(p4Metu_);
    makeTransverse(p4Metnu_);

    
    // Calculate basic variables:
    bal_ = p4Jet1_.Pt() / ptRef;
    mpf_ = 1.0 + p4CorrMet_.Vect().Dot(p4Ref_.Vect()) /ptRefSqr;

    mpf1_ = 1.0 + p4Met1_.Vect().Dot(p4Ref_.Vect()) /ptRefSqr;

    mpfn_ = p4Metn_.Vect().Dot(p4Ref_.Vect()) /ptRefSqr;

    mpfu_ = p4Metu_.Vect().Dot(p4Ref_.Vect()) /ptRefSqr;

    mpfnu_ = p4Metnu_.Vect().Dot(p4Ref_.Vect()) /ptRefSqr;


    // For MPFX: Create p4Refx by rotating the p4Ref by +90 degrees (0.5 * PI) in phi.
    p4Refx_ = p4Ref_;
    // Adjust phi by adding 0.5 * TMath::Pi() (use fmod or similar if necessary to wrap around 2pi)
    double newPhi = p4Ref_.Phi() + 0.5 * TMath::Pi();
    // Ensure the new phi is in the correct range [–pi, pi].
    newPhi = TMath::ATan2(std::sin(newPhi), std::cos(newPhi));
    p4Refx_.SetPhi(newPhi);

    mpfx_ = 1.0 + p4CorrMet_.Vect().Dot(p4Refx_.Vect()) /ptRefSqr;
    
    mpf1x_ = 1.0 + p4Met1_.Vect().Dot(p4Refx_.Vect()) /ptRefSqr;
    
    mpfnPt_ = p4Metn_.Vect().Dot(p4Refx_.Vect()) /ptRefSqr;
    
    mpfux_ = p4Metu_.Vect().Dot(p4Refx_.Vect()) /ptRefSqr;
    
    mpfnux_ = p4Metnu_.Vect().Dot(p4Refx_.Vect()) /ptRefSqr;

    // Sanity check for HDM inputs:
    double checkSum = mpf1_ + mpfn_ + mpfu_;
    if (!(std::fabs(checkSum - mpf_) < 1e-4) || isDebug_) {
        std::cout << "\nHDM input error: mpf=" << mpf_
                  << " mpf1=" << mpf1_
                  << " mpfn=" << mpfn_
                  << " mpfu=" << mpfu_ << "\n";
        std::cout << "Difference = " << (checkSum - mpf_) << "\n" << std::flush;
        // Optionally: if you wish to abort processing, you can throw an exception,
        // set an error flag, or handle the condition as appropriate.
    }
    if(isDebug_) printInputs();
}

//---------------------------------------------------------------------
// makeTransverse:
//   Sets the Pz component of a TLorentzVector to zero, keeping energy 
//   consistent with the transverse mass (if needed, recalculate energy).
//---------------------------------------------------------------------
void MathHdm::makeTransverse(TLorentzVector& vec)
{
    vec.SetPz(0.0);
    // Optionally, recalc the energy if you want the mass to be preserved 
    // for a massless particle, one might simply set E = Pt().
    // For example: vec.SetE(vec.Pt());
}

void MathHdm::printInputs() const
{
    std::cout << "MathHdm: Input 4-Vectors:\n";
    std::cout << "p4CorrMet: pt = " << p4CorrMet_.Pt()
              << ", eta = " << p4CorrMet_.Eta()
              << ", phi = " << p4CorrMet_.Phi() << "\n";
              
    std::cout << "p4Ref:     pt = " << p4Ref_.Pt()
              << ", eta = " << p4Ref_.Eta()
              << ", phi = " << p4Ref_.Phi() << "\n";
              
    std::cout << "p4Jet1:    pt = " << p4Jet1_.Pt()
              << ", eta = " << p4Jet1_.Eta()
              << ", phi = " << p4Jet1_.Phi() << "\n";
              
    std::cout << "p4Jetn:    pt = " << p4Jetn_.Pt()
              << ", eta = " << p4Jetn_.Eta()
              << ", phi = " << p4Jetn_.Phi() << "\n";

    std::cout << "p4Conservation:    pt = " << p4Conservation_.Pt()
              << ", eta = " << p4Conservation_.Eta()
              << ", phi = " << p4Conservation_.Phi() << "\n\n";
}
