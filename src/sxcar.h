#ifndef SXCAR
#define SXCAR

#include<string>

struct SXCar{
    inline SXCar(std::string const &aBrand, std::string const &aModel, int aYear, double aPrice):
        mBrand(aBrand), mModel(aModel), mYear(aYear), mPrice(aPrice){ }

    std::string mBrand;
    std::string mModel;
    int mYear;
    double mPrice;
};

#endif
