#include<functional>
#include<algorithm>

#include"cxpipesorter.h"
#include"filters.h"


CXPipeSorter::~CXPipeSorter(){
    for(auto &s : mSorts){
        delete s.mBase;
        delete s.mEq;
    }
}


bool CXPipeSorter::addSortRule(std::string const &aField, std::string const &aOp){
    auto cmp1 = make_filter(aField, aOp, "");
    auto cmp2 = make_filter(aField, "==", "");
    if(cmp1 == 0 || cmp2 == 0){
        delete cmp1;
        delete cmp2;
        return false;
    }
    mSorts.push_back({cmp1, cmp2});
    return true;
}


bool CXPipeSorter::multiKeyCmp(SXCar const *aCar1, SXCar const *aCar2){
    if(mSorts.empty())
        return false;
    auto last = mSorts.end();
    last--;
    for(auto it = mSorts.begin(); it != mSorts.end(); it++){
        if(it->mBase->cmp(*aCar1, *aCar2))
            return true;
        if(it != last && it->mEq->cmp(*aCar1, *aCar2))
            continue;
        break;
    }
    return false;
}


void CXPipeSorter::processCar(SXCar *aCar){
    using namespace std::placeholders;
    if(aCar != 0)
        sorted_insert(mSortedCars, aCar, std::bind(&CXPipeSorter::multiKeyCmp, this, _1, _2));
    else{
        for(auto const &car : mSortedCars)
            next(car);
        next(aCar);
        mSortedCars.clear();
    }
}
