#include"filters.h"

AXCmp *make_filter(std::string const &aField, std::string const &aOp, std::string const &aValue){
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
