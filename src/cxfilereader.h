#ifndef CXFILEREADER
#define CXFILEREADER

#include<stdio.h>

#include"axreader.h"

class CXFileReader : public AXReader{
public:
    CXFileReader(FILE *aFile, int aBufSize = 1024);
    ~CXFileReader();

    char next() override;

private:
    bool readNext();

    FILE *mFile;
    char const *mPos;
    char *mBuf;
    int mBufSize;
    int mActualSize;
};

#endif
