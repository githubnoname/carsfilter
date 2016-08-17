#ifndef CXPIPEPRINTER
#define CXPIPEPRINTER

#include<stdio.h>

#include"axpipejob.h"
#include"sxcar.h"


// Just prints car on FILE.

class CXPipePrinter : public AXPipeJob{
public:
    CXPipePrinter(FILE *aOut, AXPipeJob *aParent = 0) : AXPipeJob(aParent), mOut(aOut){ }
    void processCar(SXCar *aCar) override {
        if(aCar != 0)
            aCar->println(mOut);
        next(aCar);
    }

private:
    FILE *mOut;
};


#endif
