#ifndef CXPIPEFILTER
#define CXPIPEFILTER

#include"axpipejob.h"


// In terms of pipes this pipejob accepts a car, match it, and if successful sends it down the pipe.
// Otherwise, deletes car and doesn't send down.

// Filters cars by one condition
// Many condtions -> many filters in the pipe.

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
