#include"testutils.h"
#include"cxstringreader.h"
#include"cxfilereader.h"


bool test_string_reader(){
    // Test string
    std::string str = "Hello";
    CXStringReader reader(str);
    // Compare reader's resuls with original
    for(auto const &x : str)
        if(reader.next() != x)
            ERROR("Char mismatch");
    // Check that EOF works fine
    if(reader.next() != 0 || reader.next() != 0)
        ERROR("End-of-input mismatch");
    return true;
}


bool test_file_reader(){
    // Open two handlers: one is for sample, the second one is for reader
    char const *fname = "src/readers_test.cpp";
    FILE *f = fopen(fname, "r");
    FILE *fr = fopen(fname, "r");
    if(f == 0 || fr == 0)
        ERROR("Cannot open file %s", fname);
    char buf[42];
    if(fread(buf, 1, sizeof(buf), f) != sizeof(buf))
        ERROR("Cannot read the file %s", fname);
    // Test several buffer reloads
    // 16 is internal reader's buffer size.
    // This mean, it should read data from file several times.
    CXFileReader reader(fr, 16);
    for(int i = 0; i < int(sizeof(buf)); i++)
        if(buf[i] != reader.next())
            ERROR("Char mismatch");
    return true;
}


int main(){
    return !(TEST(test_string_reader) &&
             TEST(test_file_reader));
}
