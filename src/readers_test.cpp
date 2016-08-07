#include"testutils.h"
#include"cxstringreader.h"
#include"cxfilereader.h"

bool test_string_reader(){
    std::string str = "Hello";
    CXStringReader reader(str);
    for(auto const &x : str)
        if(reader.next() != x)
            ERROR("Char mismatch");
    if(reader.next() != 0 || reader.next() != 0)
        ERROR("End-of-input mismatch");
    return true;
}


bool test_file_reader(){
    char const *fname = "src/readers_test.cpp";
    FILE *f = fopen(fname, "r");
    FILE *fr = fopen(fname, "r");
    if(f == 0 || fr == 0)
        ERROR("Cannot open file %s", fname);
    char buf[42];
    if(fread(buf, 1, sizeof(buf), f) != sizeof(buf))
        ERROR("Cannot read the file %s", fname);
    //test several buffer reloads
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
