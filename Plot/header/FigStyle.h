#ifndef FIGSTYLE_H
#define FIGSTYLE_H

#include <TH1F.h>
#include <TProfile.h>
#include <TColor.h>
#include <TString.h>

class FigStyle {
public:
    // Constructor
    FigStyle();

    // Method to set histogram style
    void setStyle(TH1F* hist, const TString& title, const TString& xLabel, const TString& yLabel);

    // Method to set TProfile style
    void setStyle(TProfile* profile, const TString& title, const TString& xLabel, const TString& yLabel);
    void setStyleRatio(TProfile* profile, const TString& title, const TString& xLabel, const TString& yLabel);

    // Other utility functions can be added as needed
		void setColor(TH1F* hist, int index);
    void setColor(TProfile* profile, int index);

private:
    std::vector<std::string> hexColors;  // Vector to store hex color codes

};

#endif // FIGSTYLE_H

