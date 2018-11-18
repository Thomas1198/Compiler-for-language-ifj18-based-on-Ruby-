/**
 * @file scanner.h
 * @author Tomas Dorda (xdorda00), Jakub Konetzny ()
 * @brief
 */

#include <stdio.h>
#include "token.h"
#include "dynamic_string.h"

struct tToken get_token(FILE *source_code);

void initFile(FILE *f);

void set_content_string(Dynamic_string *string);

typedef enum 
{
	START,
	COMMENTARY,
	BACKSLASH,
	COMMENTARY,
	CHUNK_COMMENTARY,
	CHUNK_COMMENTARY_END,
	KEYWORD,
	NUMBER,
	NUMBER_DOUBLE,
	NUMBER_EXP,
	NUMBER_EXP_DONE,
	STRING_START,
	STRING,
	LESS_THAN,
	GREATER_THAN,
	EOL,
	SEMICOLON
} SCANNER_STATE;