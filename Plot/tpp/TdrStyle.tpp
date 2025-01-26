#include <iostream>
#include <cmath> // for sqrt, pow

// Helper struct for static_assert in templates
template <class>
struct always_false : std::false_type {};

// Template function to set style
template<typename T>
void TdrStyle::setStyle(T* obj) {
    if constexpr (std::is_same_v<T, TH1D>) {
        obj->SetLineWidth(3);
        obj->GetYaxis()->CenterTitle();
        obj->GetXaxis()->SetTitleOffset(1.0);
        obj->GetYaxis()->SetTitleOffset(1.0);
        obj->GetXaxis()->SetTitleSize(0.05);
        obj->GetYaxis()->SetTitleSize(0.05);
        obj->GetXaxis()->SetMoreLogLabels();
        obj->GetXaxis()->SetTitle(xTitle_.c_str());
        obj->GetYaxis()->SetTitle(yTitle_.c_str());
        if(xMin_ != default_ || xMax_ != default_){
            obj->GetXaxis()->SetRangeUser(xMin_, xMax_);
        }
        if(yMin_ != default_ || yMax_ != default_){
            obj->SetMinimum(yMin_);
            obj->SetMaximum(yMax_);
        }
    }
    else if constexpr (std::is_same_v<T, TProfile>) {
        obj->SetLineWidth(3);
        obj->SetMarkerStyle(20);
        obj->GetYaxis()->CenterTitle();
        obj->GetXaxis()->SetTitleOffset(1.0);
        obj->GetYaxis()->SetTitleOffset(1.15);
        obj->GetXaxis()->SetTitleSize(0.05);
        obj->GetYaxis()->SetTitleSize(0.07);
        obj->GetXaxis()->SetLabelSize(0.05);
        obj->GetYaxis()->SetLabelSize(0.05);
        obj->GetXaxis()->SetMoreLogLabels();
        obj->GetXaxis()->SetTitle(xTitle_.c_str());
        obj->GetYaxis()->SetTitle(yTitle_.c_str());
        if(xMin_ != default_ || xMax_ != default_){
            obj->GetXaxis()->SetRangeUser(xMin_, xMax_);
        }
        if(yMin_ != default_ || yMax_ != default_){
            obj->SetMinimum(yMin_);
            obj->SetMaximum(yMax_);
        }
    }
    else if constexpr (std::is_same_v<T, TH2D> || 
                       std::is_same_v<T, TH2F> ||
                       std::is_same_v<T, TProfile2D>
                      ) {
        obj->GetXaxis()->CenterTitle();
        obj->GetYaxis()->CenterTitle();
        obj->GetZaxis()->CenterTitle();
        obj->GetXaxis()->SetTitleOffset(1.0);
        obj->GetYaxis()->SetTitleOffset(1.6);
        obj->GetZaxis()->SetTitleOffset(1.5);
        obj->GetXaxis()->SetTitleSize(0.05);
        obj->GetYaxis()->SetTitleSize(0.05);
        obj->GetZaxis()->SetTitleSize(0.05);
        obj->GetXaxis()->SetLabelSize(0.05);
        obj->GetYaxis()->SetLabelSize(0.05);
        obj->GetZaxis()->SetLabelSize(0.05);
        obj->GetYaxis()->SetNoExponent();
        obj->GetXaxis()->SetMoreLogLabels();
        obj->GetYaxis()->SetMoreLogLabels();
        obj->GetZaxis()->SetMoreLogLabels();
        obj->GetXaxis()->SetTitle(xTitle_.c_str());
        obj->GetYaxis()->SetTitle(yTitle_.c_str());
        obj->GetZaxis()->SetTitle(zTitle_.c_str());
        if(xMin_ != default_ || xMax_ != default_){
            obj->GetXaxis()->SetRangeUser(xMin_, xMax_);
        }
        if(yMin_ != default_ || yMax_ != default_){
            obj->GetYaxis()->SetRangeUser(yMin_, yMax_);
        }
    }
    else if constexpr (std::is_same_v<T, TLegend>) {
        obj->SetFillStyle(kNone);
        obj->SetBorderSize(0);
        obj->SetTextSize(0.035);
    }
    else {
        static_assert(always_false<T>::value, "Unsupported type for setStyle");
    }
}

// Template function to set style for ratio plots
template<typename T>
void TdrStyle::setStyleRatio(T* obj) {
    if constexpr (std::is_same_v<T, TGraphErrors>) {
        if (obj->GetHistogram()) { // Check if histogram exists
            obj->GetHistogram()->SetTitle("");
            // X-axis styling
            obj->GetHistogram()->GetXaxis()->SetTitleSize(0.12);
            obj->GetHistogram()->GetXaxis()->SetLabelSize(0.12);
            obj->GetHistogram()->GetXaxis()->SetLabelFont(42);
            obj->GetHistogram()->GetXaxis()->SetTitleOffset(1.2);
            obj->GetHistogram()->GetXaxis()->SetLabelOffset(0.01);

            // Y-axis styling
            obj->GetHistogram()->GetYaxis()->SetTitleSize(0.13);
            obj->GetHistogram()->GetYaxis()->SetLabelSize(0.12);
            obj->GetHistogram()->GetYaxis()->SetLabelFont(42);
            obj->GetHistogram()->GetYaxis()->SetNdivisions(6, 5, 0);
            obj->GetHistogram()->GetYaxis()->SetTitleOffset(0.6);
            obj->GetHistogram()->GetYaxis()->SetLabelOffset(0.01);
            obj->GetHistogram()->GetYaxis()->CenterTitle();

            // Additional styling
            obj->SetMarkerStyle(20);  // Set marker style for points

            // Optional: Log scale or no exponent for x-axis
            obj->GetHistogram()->GetXaxis()->SetMoreLogLabels();
            obj->GetHistogram()->GetXaxis()->SetNoExponent();
            obj->GetHistogram()->GetXaxis()->SetTitle(xTitle_.c_str());
            obj->GetHistogram()->GetYaxis()->SetTitle(rTitle_.c_str());
            if(xMin_ != default_ || xMax_ != default_){
                obj->GetHistogram()->GetXaxis()->SetRangeUser(xMin_, xMax_);
            }
            if(rMin_ != default_ || rMax_ != default_){
                obj->GetHistogram()->GetYaxis()->SetRangeUser(rMin_, rMax_);
            }
        }
    }
    else if constexpr (std::is_same_v<T, TProfile>) {
        obj->GetXaxis()->SetTitleSize(0.12);
        obj->GetXaxis()->SetLabelSize(0.12);
        obj->GetXaxis()->SetLabelFont(42);
        obj->GetXaxis()->SetTitleOffset(1.1);
        obj->GetXaxis()->SetLabelOffset(0.01);
        obj->SetMarkerStyle(20);
        obj->GetYaxis()->SetTitleSize(0.13);
        obj->GetYaxis()->SetLabelSize(0.12);
        obj->GetYaxis()->SetLabelFont(42);
        obj->GetYaxis()->SetNdivisions(6, 5, 0);
        obj->GetYaxis()->SetTitleOffset(0.6);
        obj->GetYaxis()->SetLabelOffset(0.01);
        obj->GetYaxis()->CenterTitle();
        obj->GetXaxis()->SetMoreLogLabels();
        obj->GetXaxis()->SetNoExponent();
        obj->GetXaxis()->SetTitle(xTitle_.c_str());
        obj->GetYaxis()->SetTitle(rTitle_.c_str());
        if(xMin_ != default_ || xMax_ != default_){
            obj->GetXaxis()->SetRangeUser(xMin_, xMax_);
        }
        if(rMin_ != default_ || rMax_ != default_){
            obj->GetYaxis()->SetRangeUser(rMin_, rMax_);
        }
    }
    else {
        static_assert(always_false<T>::value, "Unsupported type for setStyleRatio");
    }
}

// Template function to set color
template<typename T>
void TdrStyle::setColor(T* obj, int index) {
    // Retrieve color from hex code
    if (hexColors.empty()) {
        std::cerr << "Error: hexColors vector is empty." << std::endl;
        return;
    }
    int color = TColor::GetColor(hexColors[index % hexColors.size()].c_str());
    if constexpr (std::is_same_v<T, TH1D> || 
                  std::is_same_v<T, TProfile> || 
                  std::is_same_v<T, TGraphErrors>) {
        obj->SetLineColor(color);
        obj->SetMarkerColor(color); // Example marker color
        // obj->SetFillColor(color); // Uncomment if needed
    }
    else {
        static_assert(always_false<T>::value, "Unsupported type for setColor");
    }
}

// Template function to set figure configuration
template <typename T>
void TdrStyle::setFigConfig(const T& params) {
    xTitle_ = params.xTitle;
    yTitle_ = params.yTitle;
    xMin_ = params.xMin;
    xMax_ = params.xMax;
    yMin_ = params.yMin;
    yMax_ = params.yMax;
    xLog_ = params.xLog;
    yLog_ = params.yLog;
    isNorm_ = params.isNorm;

    // Special handling for specific types
    if constexpr (std::is_same_v<T, FigConfigEras1D> || 
                  std::is_same_v<T, FigConfigEra1Ds> || 
                  std::is_same_v<T, FigConfigYear1Ds> || 
                  std::is_same_v<T, FigConfigErasXY> || 
                  std::is_same_v<T, FigConfigEraXYs>) {
        rTitle_ = params.rTitle;
        rMin_ = params.rMin;
        rMax_ = params.rMax;
    }
    if constexpr (std::is_same_v<T, FigConfigEra2D>) {
        zLog_ = params.zLog;
        zTitle_ = params.zTitle;
    }
}


