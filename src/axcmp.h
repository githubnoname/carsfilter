#ifndef AXCMP
#define AXCMP

#include<string>

struct SXCar;


// Comparator. It uses callable operator to compare given value with SXCar's field.
// It uses appropriated type to store value, i.e. doesn't convert value per each comparison.
// The second options is to compare two SXCars.
// The third option is to check fields supported operations.
// TODO: Refactor

class AXCmp{
public:
    virtual bool cmp(SXCar const &aCar) const = 0;
    virtual bool cmp(SXCar const &aCarLeft, SXCar const &aCarRight) const = 0;
    virtual bool isOperationSupported(std::string const &aOp) const = 0;
    virtual ~AXCmp(){ }
};

AXCmp *make_axcmp(std::string const &aField, std::string const &aOp, std::string const &aValue);

#endif
