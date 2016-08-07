#include"cxparser.h"

enum { BRAND, MODEL, YEAR, PRICE };

static void store(int aState, SXCar **aCar, std::string const &aValue){
    if(*aCar == 0)
        *aCar = new SXCar("unknown", "unknown", 0, 0);
    auto car = *aCar;
    switch(aState){
    case BRAND:
        car->mBrand = aValue;
        break;
    case MODEL:
        car->mModel = aValue;
        break;
    case YEAR:
        car->mYear = std::stoi(aValue);
        break;
    case PRICE:
        car->mPrice = std::stod(aValue);
        break;
    }
}

CXParser::~CXParser(){
    delete mReader;
}

SXCar *CXParser::next(){
    if(mLimit != -1 && mIdx >= mLimit)
        return 0;
    mIdx++;
    SXCar *car = 0;
    std::string tail;
    int state = BRAND;
    while(auto c = mReader->next()){
        switch(c){
        case ',':
            store(state, &car, tail);
            tail = "";
            ++state;
            break;
        case '\n':
            store(state, &car, tail);
            return car;
        default:
            tail.push_back(c);
            break;
        }
    }
    return car;
}
