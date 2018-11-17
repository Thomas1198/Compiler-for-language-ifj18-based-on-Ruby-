#include <stdio.h>
#include "error.h"
#include "parser.h"

int main(int argc, char* argv[]) {

    FILE *source_code;

    if(argc==1){
        source_code=stdin;
    } else if(argc==2){
        if((source_code= fopen(argv[1],"r"))==NULL){
            //TODO zkontrolovat číslo erroru
            return INTERNAL_ERROR;
        }
    } else{
        //TODO zkontrolovat číslo erroru
        return INTERNAL_ERROR;
    }

    run_parser(source_code);

    fclose(source_code);

    return 0;
}
