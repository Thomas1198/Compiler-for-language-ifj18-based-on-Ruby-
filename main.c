#include <stdio.h>
#include "error.h"
#include "parser.h"

int main(int argc, char* argv[]) {

    FILE *source_code;

    if(argc==1){
        source_code=stdin;
    } else if(argc==2){
        if((source_code= fopen(argv[1],"r"))==NULL){
            return INTERNAL_ERROR;
        }
    } else{
        return INTERNAL_ERROR;
    }


    fclose(source_code);

    return 0;
}
