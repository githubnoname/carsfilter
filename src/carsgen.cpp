#include<stdio.h>
#include<string.h>

#include"sxcar.h"
#include"cxgen.h"

static char const *ARGV0 = "";

static int help(FILE *out=stdout){
    fprintf(out, "\
Usage:\n\
    %s [-o <file>] [-l num]\n\
\n\
Options:\n\
    -h           Print this message\n\
    -o <file>    Write output to file instead stdout\n\
    -l <num>     Stop after <num> generations, default is 100\n\
", ARGV0);
    return 0;
}

static int error(char const *str, bool needHelp=false){
    fprintf(stderr, "%s\n", str);
    if(needHelp)
        help(stderr);
    return 1;
}





static void gen(FILE *out, int limit){
    static SXGenInfo geninfo = {{
            { "Lada",
              { "Samara", "Kalina", "Priora", "Granta", "Vesta" },
              1990, 2016,  // Years
              1000, 10000, // Price
            },
            { "Nissan",
              { "GT-R", "Altima", "Cube", "Versa" },
              1980, 2016,
              5000, 50000,
            },
            { "Toyota",
              { "Yaris", "Corolla", "Camry", "Tundra", "RAV4" },
              1995, 2010,
              7000, 20000,
            },
        }};
    CXGen gen(limit);
    gen.setGenInfo(geninfo);
    while(auto car = gen.next())
        car->println(out);
}


int main(int argc, char **argv){
    ARGV0 = argv[0];
    FILE *fout = stdout;
    int limit = 100;
    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-h") == 0)
            return help();
        else if(strcmp(argv[i], "-o") == 0 && argc > i + 1){
            if((fout = fopen(argv[++i], "w")) == 0)
                return error("Cannot open output file for writing");
        }
        else if(strcmp(argv[i], "-l") == 0 && argc > i + 1)
            limit = atoi(argv[++i]);
        else
            return error("Wrong format", true);
    }
    gen(fout, limit);
    return 0;
}
