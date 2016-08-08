#ifndef UTILS
#define UTILS

template<typename TSeq, typename TValue, typename TFunc>
void sorted_insert(TSeq &aSeq, TValue const &aValue, TFunc aPred){
    auto it = std::lower_bound(aSeq.begin(), aSeq.end(), aValue, aPred);
    aSeq.insert(it, aValue);
}

#endif
