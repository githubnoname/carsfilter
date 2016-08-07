#ifndef FILTERS
#define FILTERS

#include<functional>

#include"carstorage.h"

enum eop { OP_EQ, OP_NE, OP_GT, OP_LT };
enum efield { F_BRAND, F_MODEL, F_YEAR, F_PRICE };


// string -> T conversion helpers

template<typename T>
inline T from_string(std::string const &){
    return T();
}

template<>
inline std::string from_string(std::string const &aStr){
    return aStr;
}

template<>
inline int from_string(std::string const &aStr){
    return std::stoi(aStr);
}

template<>
inline double from_string(std::string const &aStr){
    return std::stod(aStr);
}


// SXCar's filed extractor

template<efield TField>
struct SXFieldInfo{ };

template<>
struct SXFieldInfo<F_BRAND>{
    using Type = std::string;
    inline std::string get(SXCar const &aCar) const {
        return aCar.mBrand;
    }
};

template<>
struct SXFieldInfo<F_MODEL>{
    using Type = std::string;
    inline std::string get(SXCar const &aCar) const {
        return aCar.mModel;
    }
};

template<>
struct SXFieldInfo<F_YEAR>{
    using Type = int;
    inline int get(SXCar const &aCar) const {
        return aCar.mYear;
    }
};

template<>
struct SXFieldInfo<F_PRICE>{
    using Type = double;
    inline double get(SXCar const &aCar) const {
        return aCar.mPrice;
    }
};


// Callable operators

struct eq{
    template<typename T>
    inline bool operator()(T const &a, T const &b) const {
        return a == b;
    }
};


struct ne{
    template<typename T>
    inline bool operator()(T const &a, T const &b) const {
        return a != b;
    }
};

struct gt{
    template<typename T>
    inline bool operator()(T const &a, T const &b) const {
        return a > b;
    }
};

struct ge{
    template<typename T>
    inline bool operator()(T const &a, T const &b) const {
        return a >= b;
    }
};

struct lt{
    template<typename T>
    inline bool operator()(T const &a, T const &b) const {
        return a < b;
    }
};

struct le{
    template<typename T>
    inline bool operator()(T const &a, T const &b) const {
        return a <= b;
    }
};


// Comparator. It uses callable operator to compare given value with SXCar's field.
// Use appropriated type to store value, i.e. doesn't convert value per each comparasion.

class AXCmp{
public:
    virtual bool cmp(SXCar const &aCar) const = 0;
    virtual ~AXCmp(){ }
};


template<efield TField, typename TFunc>
class CXCmpT : public AXCmp{
public:
    inline CXCmpT(TFunc aCmp, std::string const &aValue) : mCmp(aCmp), mValue(from_string<Type>(aValue)){ }
    inline bool cmp(SXCar const &aCar) const override {
        return mCmp(SXFieldInfo<TField>().get(aCar), mValue);
    }

private:
    using Type = typename SXFieldInfo<TField>::Type;
    TFunc mCmp;
    Type mValue;
};


// External CXCmpT's contructor. Is used to operator's type deduction.

template<efield TField, typename TFunc>
CXCmpT<TField, TFunc> *make_cmpt(TFunc aFunc, std::string const &aValue){
    return new CXCmpT<TField, TFunc>(aFunc, aValue);
}


// Comparators factory

template<efield TField>
AXCmp *make_cmp(std::string const &aOp, std::string const &aValue){
    if(aOp == "==")
        return make_cmpt<TField>(eq(), aValue);
    if(aOp == "!=")
        return make_cmpt<TField>(ne(), aValue);
    if(aOp == ">")
        return make_cmpt<TField>(gt(), aValue);
    if(aOp == ">=")
        return make_cmpt<TField>(ge(), aValue);
    if(aOp == "<")
        return make_cmpt<TField>(lt(), aValue);
    if(aOp == "<=")
        return make_cmpt<TField>(le(), aValue);
    return 0;
}


// Field filters

class AXFilter{
public:
    virtual AXCmp *getCmp() const = 0;
    virtual ~AXFilter(){ }
};

template<efield TField>
class CXFilterT : public AXFilter{
public:
    CXFilterT(std::string const &aOp, std::string const &aValue) : mCmp(make_cmp<TField>(aOp, aValue)){ }
    AXCmp *getCmp() const override {
        return mCmp;
    }

    ~CXFilterT(){
        delete mCmp;
    }

private:
    AXCmp *mCmp;
};


// Filters factory.

AXFilter *make_filter(std::string const &aField, std::string const &aOp, std::string const &aValue){
    if(aField == "brand")
        return new CXFilterT<F_BRAND>(aOp, aValue);
    else if(aField == "model")
        return new CXFilterT<F_MODEL>(aOp, aValue);
    else if(aField == "issued")
        return new CXFilterT<F_YEAR>(aOp, aValue);
    else if(aField == "price")
        return new CXFilterT<F_PRICE>(aOp, aValue);
    return 0;
}

#endif
