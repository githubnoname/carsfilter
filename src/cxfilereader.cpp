#include<algorithm>

#include"cxfilereader.h"

#define MIN_BUF_SIZE 16

CXFileReader::CXFileReader(FILE *aFile, int aBufSize) : mFile(aFile) {
    mBufSize = std::max(MIN_BUF_SIZE, aBufSize);
    mBuf = new char[mBufSize];
    mPos = mBuf;
    mActualSize = 0;
}


CXFileReader::~CXFileReader(){
    delete[] mBuf;
}


char CXFileReader::next(){
    if(mPos == mBuf + mActualSize && !readNext())
        return 0;
    return *mPos++;
}


bool CXFileReader::readNext(){
    mActualSize = fread(mBuf, 1, mBufSize, mFile);
    if(mActualSize == 0)
        return false;
    mPos = mBuf;
    return true;
}
