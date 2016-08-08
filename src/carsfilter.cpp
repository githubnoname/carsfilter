#include<stdio.h>
#include<string.h>

#include"filters.h"
#include"cxparser.h"
#include"cxfilereader.h"


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
    FILE *fout = stdout;
    FILE *fin = stdin;
    CXFilter filter;
    std::list<std::pair<std::string, std::string>> sorts;

    bool filter_continue = true;
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
            sorts.push_back({ argv[++i], "<" });
        else if(strcmp(argv[i], "--desc") == 0 && argc > i + 1)
            sorts.push_back({ argv[++i], ">" });
        else if(argc > i + 2 && filter_continue){
            filter_continue = false;
            auto field = argv[i];
            auto op = argv[++i];
            auto value = argv[++i];
            if(!filter.addComparison(field, op, value)){
                fprintf(stderr, ">>>  %s %s %s <<<\n", field, op, value);
                return error("Error in filter, check field name and its supported operations");
            }
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

    AXFilterProc *proc = 0;

    if(sorts.size() != 0){
        auto sorter = new CXSortProc(fout);
        for(auto const &s : sorts)
            if(!sorter->addSortRule(s.first, s.second))
                return error("Error in sort rule, check field name");
        proc = sorter;
    }
    else
        proc = new CXPrinterProc(fout);

    filter.run(new CXParser(new CXFileReader(fin)), proc);

    return 0;
}
