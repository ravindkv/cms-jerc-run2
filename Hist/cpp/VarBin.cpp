
#include "VarBin.h"

std::vector<double> VarBin::getBinsPt(){
    std::vector<double> binsPt = {
        15, 20, 25, 30, 35, 40, 50, 60, 75, 90, 110, 130, 175, 230,
        300, 400, 500, 600, 700, 850, 1000, 1200, 1500
    };
    /* 
    std::vector<double> binsPt = {
        1, 5, 6, 8, 10, 12, 15, 18, 21, 24, 28, 32, 37, 43, 49, 56, 64, 74, 84,
        97, 114, 133, 153, 174, 196, 220, 245, 272, 300, 330, 362, 395, 430, 468,
        507, 548, 592, 638, 686, 737, 790, 846, 905, 967, 1032, 1101, 1172, 1248,
        1327, 1410, 1497, 1588, 1684, 1784, 1890, 2000, 2116, 2238, 2366, 2500,
        2640, 2787, 2941, 3103, 3273, 3450, 3637, 3832, 4037, 4252, 4477, 4713,
        4961, 5220, 5492, 5777, 6076, 6389, 6717, 7000
    };
    */
    return binsPt;
}

std::vector<double> VarBin::getBinsEta(){
    std::vector<double> binsEta = {
        -5.191, -3.839, -3.489, -3.139, -2.964, -2.853, -2.650,
        -2.500, -2.322, -2.172, -1.930, -1.653, -1.479, -1.305,
        -1.044, -0.783, -0.522, -0.261, 0.000, 0.261, 0.522, 0.783,
        1.044, 1.305, 1.479, 1.653, 1.930, 2.172, 2.322, 2.500,
        2.650, 2.853, 2.964, 3.139, 3.489, 3.839, 5.191
    };
    /*
    std::vector<double> binsEta = {
        -5.191, -4.889, -4.716, -4.538, -4.363, -4.191, -4.013, -3.839, -3.664, -3.489,
        -3.314, -3.139, -2.964, -2.853, -2.650, -2.500, -2.322, -2.172, -2.043,
        -1.930, -1.830, -1.740, -1.653, -1.566, -1.479, -1.392, -1.305, -1.218,
        -1.131, -1.044, -0.957, -0.879, -0.783, -0.696, -0.609, -0.522, -0.435,
        -0.348, -0.261, -0.174, -0.087, 0, 0.087, 0.174, 0.261, 0.348, 0.435,
        0.522, 0.609, 0.696, 0.783, 0.879, 0.957, 1.044, 1.131, 1.218, 1.305,
        1.392, 1.479, 1.566, 1.653, 1.740, 1.830, 1.930, 2.043, 2.172, 2.322, 2.500,
        2.650, 2.853, 2.964, 3.139, 3.314, 3.489, 3.664, 3.839, 4.013, 4.191,
        4.363, 4.538, 4.716, 4.889, 5.191
    };
    */
    return binsEta;
};

std::vector<double> VarBin::getBinsPhi(){
    std::vector<double> binsPhi = {
    	-3.142, -3.054, -2.967, -2.880, -2.793, -2.705, -2.618, -2.531, -2.443,
    	-2.356, -2.269, -2.182, -2.094, -2.007, -1.920, -1.833, -1.745, -1.658,
    	-1.571, -1.484, -1.396, -1.309, -1.222, -1.134, -1.047, -0.960, -0.873,
    	-0.785, -0.698, -0.611, -0.524, -0.436, -0.349, -0.262, -0.175, -0.087,
    	0.000, 0.087, 0.175, 0.262, 0.349, 0.436, 0.524, 0.611, 0.698, 0.785,
    	0.873, 0.960, 1.047, 1.134, 1.222, 1.309, 1.396, 1.484, 1.571, 1.658,
    	1.745, 1.833, 1.920, 2.007, 2.094, 2.182, 2.269, 2.356, 2.443, 2.531,
    	2.618, 2.705, 2.793, 2.880, 2.967, 3.054, 3.142
    };
    return binsPhi;
}