#ifndef CXPIPEFILTER
#define CXPIPEFILTER

#include"axpipejob.h"

class AXCmp;


class CXPipeFilter : public AXPipeJob{
public:
    CXPipeFilter(AXCmp *aCmp, AXPipeJob *aParent = 0) : AXPipeJob(aParent), mCmp(aCmp){ }
    ~CXPipeFilter();
    void processCar(SXCar *aCar) override;

private:
    AXCmp *mCmp;
};

#endif
