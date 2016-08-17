#ifndef CXSTRINGREADER
#define CXSTRINGREADER

#include<string>

#include"axreader.h"


// Helper reader.
// Is used in tests.

class CXStringReader : public AXReader{
public:
    CXStringReader(std::string const &aStr) : mStr(aStr), mIt(mStr.begin()) { }

    char next() override {
        if(mIt != mStr.end())
            return *mIt++;
        else
            return 0;
    }

private:
    std::string mStr;
    std::string::iterator mIt;
};

#endif
