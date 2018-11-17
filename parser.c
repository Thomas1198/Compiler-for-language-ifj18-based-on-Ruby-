#include "parser.h"

void run_parser(FILE *source_code) {
    Token token_actual;
    
    while ((token_actual = get_token(source_code)).content_string != NULL) {

    }
}
