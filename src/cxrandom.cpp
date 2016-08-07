#include"cxrandom.h"

CXRandom::CXRandom() : mGen(std::random_device()()){ }


int CXRandom::genInt(int aMin, int aMax){
    if(aMin >= aMax)
        return aMin;
    std::uniform_int_distribution<int> dis(aMin, aMax);
    return dis(mGen);
}


double CXRandom::genDouble(double aMin, double aMax){
    if(aMin >= aMax)
        return aMin;
    std::uniform_real_distribution<double> dis(aMin, aMax);
    return dis(mGen);
}
