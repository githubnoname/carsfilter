#ifndef UTILS
#define UTILS


// Balanced insert in sorted sequence
template<typename TSeq, typename TValue, typename TFunc>
void sorted_insert(TSeq &aSeq, TValue const &aValue, TFunc aPred){
    auto it = std::lower_bound(aSeq.begin(), aSeq.end(), aValue, aPred);
    aSeq.insert(it, aValue);
}


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
    return atoi(aStr.c_str());
}

template<>
inline double from_string(std::string const &aStr){
    return atof(aStr.c_str());
}

#endif
