#ifndef CXPIPELINE
#define CXPIPELINE

#include<string>

#include"cxpipefilter.h"
#include"cxpipesorter.h"
#include"cxpipeprinter.h"
#include"axcmp.h"


// Pipeline helps to manage jobs in the pipe.
//

class CXPipeline{
public:
    CXPipeline() : mHead(0), mTail(0), mSorter(0){ }
    ~CXPipeline(){ delete mHead; }

    void add(AXPipeJob *aJob){
        if(mTail != 0)
            mTail->setNext(aJob);
        mTail = aJob;
        if(mHead == 0)
            mHead = mTail;
    }

    CXPipeFilter *addFilter(AXCmp *aCmp){
        auto f = new CXPipeFilter(aCmp, mTail);
        add(f);
        return f;
    }

    CXPipeFilter *addFilter(std::string const &aField, std::string const &aOp, std::string const &aValue){
        if(auto cmp = make_axcmp(aField, aOp, aValue)){
            if(cmp->isOperationSupported(aOp))
                return addFilter(cmp);
            else
                delete cmp;
        }
        return 0;
    }

    CXPipePrinter *addPrinter(FILE *aOut){
        auto p = new CXPipePrinter(aOut, mTail);
        add(p);
        return p;
    }

    CXPipeSorter *addSorter(){
        auto s = new CXPipeSorter(mTail);
        add(s);
        return s;
    }

    bool addSortRule(std::string const &aType, std::string const &aField){
        std::string op;
        if(aType == "asc")
            op = "<";
        else if(aType == "desc")
            op = ">";
        else
            return false;
        if(mSorter == 0)
            mSorter = new CXPipeSorter;
        return mSorter->addSortRule(aField, op);
    }

    void applySorter(){
        if(mSorter == 0)
            return;
        add(mSorter);
    }

    AXPipeJob *tail(){ return mTail; }
    AXPipeJob *head(){ return mHead; }

private:
    AXPipeJob *mHead, *mTail;
    CXPipeSorter *mSorter;
};

#endif
