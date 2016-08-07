#include<stdio.h>
#include<algorithm>
#include<functional>
#include<string.h>

#include"filters.h"

void printCar(SXCar const &aCar){
    printf("%s %s, %d year - $%f\n", aCar.mBrand.c_str(), aCar.mModel.c_str(), aCar.mYear, aCar.mPrice);
}

template<typename T, typename Tf>
void remove_if_not(std::list<T> &list, Tf pred){
    list.remove_if([&](T const &x){ return !pred(x); });
}

template<typename TContainer, typename TValue>
bool contains(TContainer const &aSeq, TValue const &aValue){
    return std::find(aSeq.begin(), aSeq.end(), aValue) != aSeq.end();
}


void filterCars(std::list<SXCar> &aCars, std::list<AXFilter*> const &aFilters){
    using namespace std::placeholders;
    remove_if_not(aCars, [&](SXCar const &car){
            return std::all_of(aFilters.begin(), aFilters.end(), [&](AXFilter *filter){
                    return filter->getCmp()->cmp(car);
                });
        });
}


char const *ARGV0 = "";


int help(FILE *out=stdout){
    fprintf(out, "Usage:\n");
    fprintf(out, "    %s [-f <file>] [-o <file>] [-l <num>] [-h] [FILTER [AND FILTER [...]]]\n", ARGV0);
    fprintf(out, "\nOptions:\n");
    fprintf(out, "    -h            Print this message\n");
    fprintf(out, "    -f <file>     Read cars info from specified file. Default is stdin.\n");
    fprintf(out, "    -o <file>     Write cars info to specified file. Default is stdout.\n");
    fprintf(out, "    -l <num>      Read just <num> cars from input. -1 means unlimited, it is default.\n");
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


int error(char const *str, bool needHelp=false){
    fprintf(stderr, "%s\n", str);
    if(needHelp)
        help(stderr);
    return 1;
}


int main(int argc, char **argv){
    ARGV0 = argv[0];
    std::list<SXCar> cars;
    std::list<AXFilter*> filters;
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

    cars = loadCars("");

    for(auto const &c : cars)
        printCar(c);

    filterCars(cars, filters);

    printf("\n---\n");
    for(auto const &c : cars)
        printCar(c);

    return 0;
}
