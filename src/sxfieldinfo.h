#ifndef SXFIELDINFO
#define SXFIELDINFO

#include"sxcar.h"


enum efield { F_BRAND, F_MODEL, F_YEAR, F_PRICE };

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


#endif
