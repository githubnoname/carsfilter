#ifndef CXRANDOM
#define CXRANDOM

#include<random>

class CXRandom{
public:
    CXRandom();
    int genInt(int aMin, int aMax);
    double genDouble(double aMin, double aMax);

private:
    std::mt19937 mGen;
};

#endif
