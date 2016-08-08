#ifndef CXPIPESORTER
#define CXPIPESORTER

#include<list>
#include<vector>

#include"axpipejob.h"
#include"utils.h"

class AXCmp;
class SXCar;


// In multi-key sorting, we have to do two compares per each cars fields.
// First one is the usual, second one is eqaulity check to start compare by next key.
//
// e.g. the first sort key is issuance date desc, the second one is price asc
// 1: usual comparison (price)
// 2: equality comparison (price)
// 3: comparison by second key (issuance date)
//
//        compare(car1. car2):
// /* 1 */    if car1.mYear > car2.mYear then
//                return true
// /* 2 */    if car1.mYear == car2.mYear then
// /* 3 */        return car1.mPrice < car2.mPrice
//            return false;
//
// Pair of comparators is stored in SXMultiSort;
// Algorithm for N-keys comparisin is implemented in multiKeyCmp

class CXPipeSorter : public AXPipeJob{
public:
    CXPipeSorter(AXPipeJob *aParent = 0) : AXPipeJob(aParent){ }
    ~CXPipeSorter();
    bool addSortRule(std::string const &aField, std::string const &aOp);
    void processCar(SXCar *aCar) override;

private:
    struct SXMultiSort{
        AXCmp *mBase;
        AXCmp *mEq;
    };
    bool multiKeyCmp(SXCar const *aCar1, SXCar const *aCar2);

    std::list<SXMultiSort> mSorts;
    std::vector<SXCar*> mSortedCars;
};

#endif
