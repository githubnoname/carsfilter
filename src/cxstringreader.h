#ifndef CXSTRINGREADER
#define CXSTRINGREADER

#include<string>

#include"axreader.h"

class CXStringReader : public AXReader{
public:
    CXStringReader(std::string const &aStr) : mStr(aStr), mIt(mStr.begin()) { }

    char next() override;

private:
    std::string mStr;
    std::string::iterator mIt;
};

#endif
