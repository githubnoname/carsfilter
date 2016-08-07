#ifndef CXGEN
#define CXGEN

#include<vector>
#include<string>


struct SXCar;

struct SXBrandInfo{
    std::string mName;
    std::vector<std::string> mModels;
    int mYearMin, mYearMax;
    double mPriceMin, mPriceMax;
};

struct SXGenInfo{
    std::vector<SXBrandInfo> mBrands;
};


class CXGen{
public:
    CXGen(int aLimit) : mLimit(aLimit), mIdx(0){ }
    SXCar *next();

    void setGenInfo(SXGenInfo const &aGenInfo){
        mGenInfo = aGenInfo;
    }


private:
    int mLimit;
    int mIdx;
    SXGenInfo mGenInfo;
};

#endif
