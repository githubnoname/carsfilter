#include<list>
#include"testutils.h"
#include"cxparser.h"
#include"cxstringreader.h"


static std::string car_to_string(SXCar *aCar){
    if(aCar == 0)
        return "NULL";
    return aCar->mBrand + ", " + aCar->mModel + ", " + std::to_string(aCar->mYear) + ", " + std::to_string(aCar->mPrice);
}


// Compare parser's values with expected ones
static bool cmp(CXParser *aParser, std::list<SXCar*> const &aExpected){
    for(auto const &e : aExpected){
        auto car = aParser->next();
        if(car != e && *car != *e)
            ERROR("Cars mismatch, expected: %s, got: %s", car_to_string(e).c_str(), car_to_string(car).c_str());
    }
    return true;
}


static std::string const input = "Lada;Granta;2010;7000.42\n\
BMW;X5;2015;50000\n\
;;100\n\
\n\
";


// Test whole input
bool test_parser(){
    // Build expected table of cars
    std::list<SXCar*> expected = {
        new SXCar("Lada", "Granta", 2010, 7000.42),
        new SXCar("BMW", "X5", 2015, 50000),
        new SXCar("", "", 100, 0),                 // bad field
        new SXCar("", "unknown", 0, 0),            // empty field
        0,                                         // end-of-input
    };

    // Create reader and parser
    CXParser parser(new CXStringReader(input));
    // Compare
    return cmp(&parser, expected);
}

// Test limited parser (2 first values)
bool test_limited_parser(){
    std::list<SXCar*> expected = {
        new SXCar("Lada", "Granta", 2010, 7000.42),
        new SXCar("BMW", "X5", 2015, 50000),
        0,                                         // Limit
        0,                                         // Limit
    };
    CXParser parser(new CXStringReader(input), 2);
    return cmp(&parser, expected);
}

int main(){
    return !(TEST(test_parser) &&
             TEST(test_limited_parser));
}
