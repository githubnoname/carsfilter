#include"filters.h"
#include"cxparser.h"
#include"utils.h"


CXFilter::~CXFilter(){
    for(auto &cmp : mCmps)
        delete cmp;
}


bool CXFilter::addComparison(std::string const &aField, std::string const &aOp, std::string const &aValue){
    if(auto cmp = make_filter(aField, aOp, aValue)){
        mCmps.push_back(cmp);
        return true;
    }
    return false;
}


void CXFilter::run(CXParser *aParser, AXFilterProc *aProc){
    while(auto car = aParser->next()){
        if(match(*car, mCmps))
            aProc->processCar(car);
        else
            delete car;
    }
    delete aProc;
    delete aParser;
}


void CXPrinterProc::processCar(SXCar *aCar){
    aCar->println(mOut);
    delete aCar;
}

// In multi-key sorting, we have to do two compares per each cars fields.
// First one is the usual, second one is eqaulity check to start compare by next key.
//
// e.g. the first sort key is issuance date desc, the second one is price asc
// 1: usual comparison (price)
// 2: equality comparison (price)
// 3: comparison by second key (issuance date)
//
//        compare(car1. car2):
// /* 1 */    if car1.mYear > car2.mYear then
//                return true
// /* 2 */    if car1.mYear == car2.mYear then
// /* 3 */        return car1.mPrice < car2.mPrice
//            return false;

CXSortProc::~CXSortProc(){
    for(auto &car : mSortedCars){
        car->println(mOut);
        delete car;
    }
    for(auto &s : mSorts){
        delete s.mBase;
        delete s.mEq;
    }
}


bool CXSortProc::addSortRule(std::string const &aField, std::string const &aOp){
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


bool CXSortProc::multiKeyCmp(SXCar const *aCar1, SXCar const *aCar2){
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


void CXSortProc::processCar(SXCar *aCar){
    using namespace std::placeholders;
    sorted_insert(mSortedCars, aCar, std::bind(&CXSortProc::multiKeyCmp, this, _1, _2));
}


AXCmp *make_filter(std::string const &aField, std::string const &aOp, std::string const &aValue){
    if(aField == "brand")
        return make_cmp<F_BRAND>(aOp, aValue);
    else if(aField == "model")
        return make_cmp<F_MODEL>(aOp, aValue);
    else if(aField == "issued")
        return make_cmp<F_YEAR>(aOp, aValue);
    else if(aField == "price")
        return make_cmp<F_PRICE>(aOp, aValue);
    return 0;
}
