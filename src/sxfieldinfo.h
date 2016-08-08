#ifndef SXFIELDINFO
#define SXFIELDINFO

#include<list>
#include<string>

#include"sxcar.h"


enum efield { F_BRAND, F_MODEL, F_YEAR, F_PRICE };


// SXCar's field extractor

template<efield TField>
struct SXFieldInfo{ };

template<>
struct SXFieldInfo<F_BRAND>{
    using Type = std::string;
    inline std::string get(SXCar const &aCar) const {
        return aCar.mBrand;
    }
    std::list<std::string> ops() const { return { "==" }; }
};

template<>
struct SXFieldInfo<F_MODEL>{
    using Type = std::string;
    inline std::string get(SXCar const &aCar) const {
        return aCar.mModel;
    }
    std::list<std::string> ops() const { return { }; }
};

template<>
struct SXFieldInfo<F_YEAR>{
    using Type = int;
    inline int get(SXCar const &aCar) const {
        return aCar.mYear;
    }
    std::list<std::string> ops() const { return { "==", "!=", ">", ">=", "<", "<=" }; }
};

template<>
struct SXFieldInfo<F_PRICE>{
    using Type = double;
    inline double get(SXCar const &aCar) const {
        return aCar.mPrice;
    }
    std::list<std::string> ops() const { return { ">", "<" }; }
};

#endif
