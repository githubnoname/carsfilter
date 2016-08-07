#ifndef SXCAR
#define SXCAR

#include<string>
#include<limits>
#include<cmath>

struct SXCar{
    inline SXCar(std::string const &aBrand, std::string const &aModel, int aYear, double aPrice):
        mBrand(aBrand), mModel(aModel), mYear(aYear), mPrice(aPrice){ }

    bool operator==(SXCar const &aCar){
        return (mBrand == aCar.mBrand &&
                mModel == aCar.mModel &&
                mYear  == aCar.mYear  &&
                std::abs(mPrice - aCar.mPrice) <= std::numeric_limits<double>::epsilon());
    }

    bool operator!=(SXCar const &aCar){
        return !(*this == aCar);
    }

    std::string mBrand;
    std::string mModel;
    int mYear;
    double mPrice;
};

#endif
