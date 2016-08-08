#include<stdio.h>
#include<algorithm>
#include<functional>
#include<string.h>

#include"filters.h"
#include"cxparser.h"
#include"cxfilereader.h"
#include"utils.h"


// Optimized version of filtering without sorting. Filter data line by line.
static void filter(CXParser *aParser, FILE *aOut, std::list<AXCmp*> const &aFilters){
    while(auto car = aParser->next()){
        if(match(*car, aFilters))
            car->println(aOut);
        delete car;
    }
}


// In multi-key sorting, we have to do two compares per each cars fields.
// First one is the usual, second one is eqaulity check to start compare by next key.
//
// e.g. first sort key is price asc, second one is issuance date desc
// 1: usual comparation (price)
// 2: equality comparation (price)
// 3: comparation by second key (issuance date)
//
//        compare(car1. car2):
// /* 1 */    if car1.mPrice > car2.mPrice then
//                return true
// /* 2 */    if car1.mPrice == car2.mPrice then
// /* 3 */        return car1.mYear < car2.mYear
//            return false;
struct SXMultiSort{
    AXCmp *mBase;
    AXCmp *mEq;
    // ~SXMultiSort(){
    //     printf("delete %p %p\n", mBase, mEq);
    //     delete mBase;
    //     delete mEq;
    // }
};


bool multiKeyCmp(SXCar const aCar1, SXCar const &aCar2, std::list<SXMultiSort> aCmps){
    if(aCmps.empty())
        return false;
    if(aCmps.front().mBase->cmp(aCar1, aCar2))
        return true;
    if(aCmps.size() > 1 && aCmps.front().mEq->cmp(aCar1, aCar2)){
        aCmps.pop_front();
        return multiKeyCmp(aCar1, aCar2, aCmps);
    }
    return false;
}


// Filtering and sorting. Load all filtered cars in memory, sort, and print.
static void filterAndSort(CXParser *aParser, FILE *aOut, std::list<AXCmp*> const &aFilters, std::list<SXMultiSort> aSorts){
    std::vector<SXCar*> cars;
    while(auto car = aParser->next()){
        if(match(*car, aFilters)){
            sorted_insert(cars, car, [&](SXCar *a, SXCar *b) -> bool {
                    return multiKeyCmp(*a, *b, aSorts);
                });
        }
        else
            delete car;
    }
    for(auto &car : cars){
        car->println(aOut);
        delete car;
    }
}


static char const *ARGV0 = "";

static int help(FILE *out=stdout){
    fprintf(out, "Usage:\n");
    fprintf(out, "    %s [-f <file>] [-o <file>] [-l <num>] [-h] [FILTER [AND FILTER [...]]]\n", ARGV0);
    fprintf(out, "\nOptions:\n");
    fprintf(out, "    -h              Print this message\n");
    fprintf(out, "    -f <file>       Read cars info from specified file. Default is stdin.\n");
    fprintf(out, "    -o <file>       Write cars info to specified file. Default is stdout.\n");
    fprintf(out, "    -l <num>        Read just <num> cars from input. -1 means unlimited, it is default.\n");
    fprintf(out, "    --asc <field>   Ascending sort by field.\n");
    fprintf(out, "    --desc <field>  Descending sort by field.\n");
    fprintf(out, "\nFilter format:\n");
    fprintf(out, "    filter = field operation value]\n");
    fprintf(out, "    field = brand | model | issued | price\n");
    fprintf(out, "    Supported operations per field:\n");
    fprintf(out, "        brand: ==\n");
    fprintf(out, "        model:\n");
    fprintf(out, "        issued: ==, !=, >, >=, <, <=\n");
    fprintf(out, "        price: >, <\n");
    fprintf(out, "    Value is value to compare with.\n");
    return 0;
}


static int error(char const *str, bool needHelp=false){
    fprintf(stderr, "%s\n", str);
    if(needHelp)
        help(stderr);
    return 1;
}


int main(int argc, char **argv){
    ARGV0 = argv[0];
    std::list<SXCar> cars;
    std::list<AXCmp*> filters;
    std::list<SXMultiSort> sorts;
    FILE *fout = stdout;
    FILE *fin = stdin;
    bool filter_continue = false;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-h") == 0)
            return help();
        else if(strcmp(argv[i], "-f") == 0 && argc > i + 1){
            if((fin = fopen(argv[++i], "r")) == 0)
                return error("Cannot open input file for reading");
        }
        else if(strcmp(argv[i], "-o") == 0 && argc > i + 1){
            if((fout = fopen(argv[++i], "w")) == 0)
                return error("Cannot open output file for writing");
        }
        else if(strcmp(argv[i], "--asc") == 0 && argc > i + 1)
            sorts.push_back({ make_filter(argv[++i], "<", "0"),
                              make_filter(argv[i], "==", "0") });
        else if(strcmp(argv[i], "--desc") == 0 && argc > i + 1)
            sorts.push_back({ make_filter(argv[++i], ">", "0"),
                              make_filter(argv[i], "==", "0") });
        else if(argc > i + 2 && (filters.size() == 0 || filter_continue)){
            filter_continue = false;
            auto field = argv[i];
            auto op = argv[++i];
            auto value = argv[++i];
            filters.push_back(make_filter(field, op, value));
            if(argc > i + 1){
                if(strcmp(argv[i + 1], "AND") == 0){
                    i++;
                    filter_continue = true;
                }
            }
        }
        else
            return error("Wrong format", true);
    }

    CXParser p(new CXFileReader(fin));

    if(sorts.size() == 0)
        filter(&p, fout, filters);
    else
        filterAndSort(&p, fout, filters, sorts);

    for(auto &f : filters)
        delete f;

    return 0;
}
