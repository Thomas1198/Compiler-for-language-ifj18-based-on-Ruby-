#include "parser.h"

void run_parser(FILE *source_code) {
    struct tToken token_actual;
    tDList token_list;

    while ((token_actual = get_token(source_code)).content_string != NULL) {
        DLInsertLast(&token_list, token_actual);
    }
}
