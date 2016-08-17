#include<stdio.h>
#include<string.h>

#include"cxparser.h"
#include"cxfilereader.h"
#include"cxpipeline.h"


// Utility for filtering and sorting cars

static char const *ARGV0 = "";

static int help(FILE *out=stdout){
    fprintf(out, "Usage:\n");
    fprintf(out, "    %s [-f <file>] [-o <file>] [-l <num>] [-h] [--filter <filter>]* [--asc | --desc <field>]*\n", ARGV0);
    fprintf(out, "\nOptions:\n");
    fprintf(out, "    -h              Print this message\n");
    fprintf(out, "    -f <file>       Read cars info from specified file. Default is stdin.\n");
    fprintf(out, "    -o <file>       Write cars info to specified file. Default is stdout.\n");
    fprintf(out, "    -l <num>        Read just <num> cars from input. -1 means unlimited, it is default.\n");
    fprintf(out, "    --asc <field>   Ascending sort by field.\n");
    fprintf(out, "    --desc <field>  Descending sort by field.\n");
    fprintf(out, "    --filter <fi... Add filter.");
    fprintf(out, "\nFilter format:\n");
    fprintf(out, "    filter = field operation value\n");
    fprintf(out, "    field = brand | model | issued | price\n");
    fprintf(out, "    Supported operations per field:\n");
    fprintf(out, "        brand: ==\n");
    fprintf(out, "        model:\n");
    fprintf(out, "        issued: ==, !=, >, >=, <, <=\n");
    fprintf(out, "        price: >, <\n");
    fprintf(out, "    Value is value to compare with.\n");
    fprintf(out, "\n");
    fprintf(out, "All sorting options are combined into a multi key sorting rule, that applies after filters\n");
    fprintf(out, "This mean, this kind of sorting groups the result by primary keys.\n");
    return 0;
}


static int error(char const *str, bool needHelp=false){
    fprintf(stderr, "%s\n", str);
    if(needHelp)
        help(stderr);
    return 1;
}


// Creates filters directly in the pipeline
// Groups sorters in the intermediate multi-key sorter
// Applies sorter to the end of pipeline
// Applies printer to the end of pipeline
// Reads cars from parser and process them one by one in the pipeline

int main(int argc, char **argv){
    ARGV0 = argv[0];
    FILE *fout = stdout;
    FILE *fin = stdin;

    CXPipeline pipeline;

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
        else if((strcmp(argv[i], "--asc") == 0 || strcmp(argv[i], "--desc") == 0) && argc > i + 1){
            auto type = argv[i] + 2;
            auto field = argv[++i];
            if(!pipeline.addSortRule(type, field)){
                fprintf(stderr, ">>> %s <<<\n", argv[i]);
                return error("Error in sort rule, check field name");
            }
        }
        else if(strcmp(argv[i], "--filter") == 0 && argc > i + 3){
            auto field = argv[++i];
            auto op = argv[++i];
            auto value = argv[++i];
            if(!pipeline.addFilter(field, op, value)){
                fprintf(stderr, ">>>  %s %s %s <<<\n", field, op, value);
                return error("Error in filter, check field name and its supported operations");
            }
        }
        else
            return error("Wrong format", true);
    }

    pipeline.applySorter();
    pipeline.addPrinter(fout);

    CXParser p(new CXFileReader(fin));
    SXCar *car = 0;
    do{
        car = p.next();
        pipeline.head()->processCar(car);
    } while(car != 0);

    fclose(fin);
    fclose(fout);
    return 0;
}
