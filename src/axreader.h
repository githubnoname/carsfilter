#ifndef AXREADER
#define AXREADER


// Reader interface
// 0 byte means that reading is finished.
// Bad idea for binary files, but is very simple.

class AXReader{
public:
    virtual char next() = 0;
    virtual ~AXReader(){ }
};

#endif
