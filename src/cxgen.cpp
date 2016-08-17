#include<random>

#include"cxgen.h"
#include"sxcar.h"
#include"cxrandom.h"


template<typename T>
T const* random_from_vector(CXRandom &rnd, std::vector<T> const &aVector){
    if(aVector.size() == 0)
        return 0;
    return &aVector[rnd.genInt(0, int(aVector.size() - 1))];
}


SXCar *CXGen::next(){
    if(mIdx >= mLimit)
        return 0;
    mIdx++;
    CXRandom rnd;
    auto brandInfo = random_from_vector(rnd, mGenInfo.mBrands);
    if(brandInfo == 0)
        return 0;
    auto model = random_from_vector(rnd, brandInfo->mModels);
    if(model == 0)
        return 0;
    int year = rnd.genInt(brandInfo->mYearMin, brandInfo->mYearMax);
    double price = rnd.genDouble(brandInfo->mPriceMin, brandInfo->mPriceMax);
    return new SXCar(brandInfo->mName, *model, year, price);
}
