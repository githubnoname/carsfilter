#ifndef CXPARSER
#define CXPARSER

#include"sxcar.h"
#include"axreader.h"


// Parser gets bytes by reader::next() one by one.
// Returns pointer to SXCar or NULL if reading is finished.
// Uses semicolon ';' as fields separator.
// Uses newline as cars separator.

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
