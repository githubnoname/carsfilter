#include"cxpipefilter.h"
#include"axcmp.h"


CXPipeFilter::~CXPipeFilter(){
    delete mCmp;
}


void CXPipeFilter::processCar(SXCar *aCar){
    if(aCar == 0 || mCmp->cmp(*aCar))
        next(aCar);
    else
        delete aCar;
}
