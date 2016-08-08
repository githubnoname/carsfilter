#include"axcmp.h"
#include"sxfieldinfo.h"
#include"utils.h"

// Callable operators

namespace {
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
}


template<efield TField, typename TFunc>
class CXCmpT : public AXCmp{
public:
    inline CXCmpT(TFunc aCmp, std::string const &aValue) : mCmp(aCmp), mValue(from_string<Type>(aValue)){ }
    inline bool cmp(SXCar const &aCar) const override {
        return mCmp(SXFieldInfo<TField>().get(aCar), mValue);
    }
    inline bool cmp(SXCar const &aCarLeft, SXCar const &aCarRight) const override {
        return mCmp(SXFieldInfo<TField>().get(aCarLeft), SXFieldInfo<TField>().get(aCarRight));
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


// Comparators factory. Step 2. encapsulate operation

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


// Comparators factory. Step 1: encapsulate field

AXCmp *make_axcmp(std::string const &aField, std::string const &aOp, std::string const &aValue){
    if(aField == "brand")
        return make_cmp<F_BRAND>(aOp, aValue);
    else if(aField == "model")
        return make_cmp<F_MODEL>(aOp, aValue);
    else if(aField == "issued")
        return make_cmp<F_YEAR>(aOp, aValue);
    else if(aField == "price")
        return make_cmp<F_PRICE>(aOp, aValue);
    return 0;
}
