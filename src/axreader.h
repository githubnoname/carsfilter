#ifndef AXREADER
#define AXREADER

// Reader interface

class AXReader{
public:
    virtual char next() = 0;
    virtual ~AXReader(){ }
};

#endif
