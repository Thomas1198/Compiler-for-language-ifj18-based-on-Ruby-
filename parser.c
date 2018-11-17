#include "parser.h"

void run_parser(FILE *source_code) {
    struct tToken token_actual;
    tToken_list token_list;

    while ((token_actual = get_token(source_code)).content_string != NULL) {
        if (add_to_token_list(&token_list, token_actual)) {
            //TODO uvolnit pamet
            exit(INTERNAL_ERROR);
        }
    }
}
