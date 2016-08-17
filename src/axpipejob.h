#ifndef AXPIPEJOB
#define AXPIPEJOB

#include"sxcar.h"


// Allows to build chains (or pipes) of processors for cars.
// Deletes car on end of the pipe.

class AXPipeJob{
public:
    AXPipeJob(AXPipeJob *aParent = 0) : mNext(0){
        if(aParent != 0)
            aParent->setNext(this);
    }
    virtual ~AXPipeJob(){ delete mNext; }
    virtual void processCar(SXCar *) = 0;
    virtual void setNext(AXPipeJob *aNext){ mNext = aNext; }

protected:
    virtual void next(SXCar *aCar){
        if(mNext != 0)
            mNext->processCar(aCar);
        else
            delete aCar;
    }

    AXPipeJob *mNext;
};

#endif
