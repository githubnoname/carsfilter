#include"cxstringreader.h"

char CXStringReader::next(){
    if(mIt != mStr.end())
        return *mIt++;
    else
        return 0;
}
