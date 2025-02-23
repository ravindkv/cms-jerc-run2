#pragma once

#include <iostream>

using namespace std;

class GlobalFlag{
  public: 
    GlobalFlag(std::string oName);
    ~GlobalFlag();
    
    bool isDebug;

    bool isPrintConfig;

    //Channels
    bool isL3Res;
    
    void printFlag();

  private:
    string outName;
};
