#ifndef CXPARSER
#define CXPARSER

#include"sxcar.h"
#include"axreader.h"

class CXParser{
public:
    CXParser(AXReader *aReader, int aLimit = -1) : mReader(aReader), mLimit(aLimit), mIdx(0){ }
    ~CXParser();
    SXCar *next();

protected:
    AXReader *mReader;
    int mLimit;
    int mIdx;
};

#endif
