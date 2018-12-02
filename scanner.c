/**
 * @file scanner.c
 * @author Patrik Strnad (xstrna11)
 * @brief Error codes constants and function to print errors.
 */


#include "scanner.h"



//end of includes
bool new_line = false;
int start_token = -1;

//FILE *source_file;

struct tToken get_token(FILE *source_file) {
    if (source_file == NULL) {
        ErrorPrint(INTERNAL_ERROR, "[scanner.c][get_token] failed mount");
    }

    struct tToken token;
    init_token(&token);

    //Setuji current_state na DEFAULT
    SCANNER_STATE current_state = START;

    //Declaration of the scanner char
    int c = 0;

    while (true) {

        if (start_token < 2) {
            start_token++;
        }

        c = getc(source_file);

        switch (current_state) {
            case (START):

                if (c == '\n') {
                    current_state = EOL;
                } else if (c == '/') {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                    token.set_type_of_token = CHAR_OPERATOR_DIV;
                    return token;
                } else if (c == '#') {
                    current_state = COMMENTARY;
                } else if (c == '_' || isalpha(c)) {
                    // if (new_line)
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                    current_state = KEYWORD;
                } else if (isdigit(c)) {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                    current_state = NUMBER;
                } else if (c == '<') {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                    current_state = LESS_THAN;
                } else if (c == '>') {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                    current_state = GREATER_THAN;
                } else if (c == '=') {
                    if ((start_token == 0) || (new_line)) {
                        current_state = MIGHTBECOMMENT;
                    } else {
                        dynamic_string_add_char(token.content_string, (char) tolower(c));
                        current_state = EQUALS;
                    }
                } else if (isspace(c)) {
                    current_state = START;
                } else if (c == '+') {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                    token.set_type_of_token = CHAR_OPERATOR_PLUS;
                    return token;
                } else if (c == '-') {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                    token.set_type_of_token = CHAR_OPERATOR_MINUS;
                    return token;
                } else if (c == '*') {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                    token.set_type_of_token = CHAR_OPERATOR_MUL;
                    return token;
                } else if (c == ',') {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                    token.set_type_of_token = CHAR_COMMA;
                    return token;
                } else if (c == '(') {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                    token.set_type_of_token = CHAR_LEFT_BRACKET;
                    return token;
                } else if (c == ')') {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                    token.set_type_of_token = CHAR_RIGHT_BRACKET;
                    return token;
                } else if (c == '"') {
                    current_state = STRING_START;
                } else if (c == '!') {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                    current_state = EXCLAMATION;
                } else if (c == EOF) {
                    dynamic_string_add_const_str(token.content_string, "EOF");
                    token.set_type_of_token = CHAR_EOF;
                    return token;
                } else {
                    ErrorPrint(SCANNER_ERROR, "[scanner.c][get_token] CASE START");
                }
                break;

            case (KEYWORD): {
                if (isalnum(c) || c == '_') {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                    current_state = KEYWORDLONGER;
                } else {
                    ungetc(c, source_file);
                    return process_identifier(token);
                }

            }
                break;

            case (KEYWORDLONGER):
                if (isalnum(c) || c == '_') {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                } else {
                    ungetc(c, source_file);
                    return process_identifier(token);
                }

                break;


            case (EOL):
                if (isspace(c)) {
                    break;
                }
                if (c == '=') {
                    new_line = true;
                } else {
                    new_line = false;
                }
                ungetc(c, source_file);
                dynamic_string_add_const_str(token.content_string, "\\n");
                token.set_type_of_token = CHAR_EOL;
                return token;

            case (EXCLAMATION):
                if (c == '=') {
                    dynamic_string_add_char(token.content_string, (char) c);
                    token.set_type_of_token = CHAR_NEQ;
                    return token;
                } else {
                    ErrorPrint(SCANNER_ERROR, "[scanner.c][get_token] CASE EXCLAMATION");
                }


            case (NUMBER):
                if (isdigit(c)) {
                    dynamic_string_add_char(token.content_string, (char) c);

                } else if (c == '.') {
                    dynamic_string_add_char(token.content_string, '.');
                    current_state = NUMBER_DEC;
                } else if (tolower(c) == 'e') {
                    current_state = NUMBER_EXP;
                } else {
                    ungetc(c, source_file);
                    return process_integer(token);
                }

                break;

            case (NUMBER_DEC):

                if (isdigit(c)) {
                    current_state = NUMBER_DOUBLE;
                    dynamic_string_add_char(token.content_string, (char) c);
                } else {
                    ungetc(c, source_file);
                    return process_decimal(token);
                }
                break;

            case (NUMBER_DOUBLE):

                if (isdigit(c)) {
                    dynamic_string_add_char(token.content_string, (char) c);

                } else if (tolower(c) == 'e') {
                    current_state = NUMBER_EXP;
                    dynamic_string_add_char(token.content_string, (char) c);
                } else {
                    ungetc(c, source_file);
                    return process_decimal(token);
                }
                break;

            case (NUMBER_EXP):
                if (isdigit(c)) {
                    current_state = NUMBER_EXP_DONE;
                    dynamic_string_add_char(token.content_string, (char) c);
                } else if (c == '-' || c == '+') {
                    current_state = NUMBER_EXP_SIGN;
                    dynamic_string_add_char(token.content_string, (char) c);
                } else {
                    ErrorPrint(SCANNER_ERROR, "[scanner.c][get_token][NUMBER_EXP]");
                }
                break;

            case (NUMBER_EXP_DONE):
                if (isdigit(c)) {
                    dynamic_string_add_char(token.content_string, (char) c);
                } else {
                    ungetc(c, source_file);
                    return process_decimal(token);
                }

                break;

            case (NUMBER_EXP_SIGN):
                if (isdigit(c)) {
                    current_state = NUMBER_EXP_DONE;
                    dynamic_string_add_char(token.content_string, (char) c);
                } else {
                    ErrorPrint(SCANNER_ERROR, "[scanner.c][get_token][NUMBER_EXP_SIGN]");
                }

                break;


            case (COMMENTARY):
                if (c == '\n' || c == EOF) {
                    current_state = START;
                    ungetc(c, source_file);
                }
                break;

            case (STARTCHUNKCOMMENTARY): {
                if (c == '=') {
                    current_state = STARTCHUNKCOMMENTARYCONTINUE;
                }
            }
                break;

            case (STARTCHUNKCOMMENTARYCONTINUE):
                if (isalpha(c)) {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                } else if (!isalpha(c)) {
                    int result = process_commentary(token);
                    if (result == 1) {
                        current_state = STARTCHUNKCOMMENTARY;
                        dynamic_string_clear(token.content_string);
                    } else if (result == 2) {
                        dynamic_string_clear(token.content_string);
                        current_state = START;
                    } else {
                        dynamic_string_clear(token.content_string);
                        current_state = STARTCHUNKCOMMENTARY;
                    }
                }
                break;

            case (EQUALS):
                if (c == '=') {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                    token.set_type_of_token = CHAR_DOUBLEEQ;
                    return token;
                } else {
                    ungetc(c, source_file);
                    token.set_type_of_token = CHAR_EQUALS;
                    return token;
                }
                break;

            case (MIGHTBECOMMENT):
                if (isalpha(c)) {
                    dynamic_string_add_char(token.content_string, (char) tolower(c));
                } else if (isspace(c)) {
                    int result = process_commentary(token);
                    if (result == 1) {
                        current_state = STARTCHUNKCOMMENTARY;
                        dynamic_string_clear(token.content_string);
                    } else if (result == 2) {
                        dynamic_string_clear(token.content_string);
                        current_state = START;
                    } else {
                        fseek(source_file, -strlen((char *) token.content_string), SEEK_CUR);
                        dynamic_string_add_char(token.content_string, '=');
                        token.set_type_of_token = CHAR_EQUALS;
                        return token;
                    }
                }
                break;

            case (STRING_START):
                if (c < 32) {
                    ErrorPrint(SCANNER_ERROR, "[scanner.c][get_token][STRING_START]");
                } else if (c == '"') {
                    token.set_type_of_token = LITERAL_STRING;
                    return token;
                } else {
                    dynamic_string_add_char(token.content_string, (char) c);
                }
                break;


            case LESS_THAN: {
                if (c == '=') {
                    token.set_type_of_token = CHAR_LEQ;
                    return token;
                } else {
                    ungetc(c, source_file);
                    token.set_type_of_token = CHAR_LT;
                    return token;
                }

            }

            case GREATER_THAN: {
                if (c == '=') {
                    token.set_type_of_token = CHAR_GEQ;
                    return token;
                } else {
                    ungetc(c, source_file);
                    token.set_type_of_token = CHAR_GT;
                    return token;
                }

            }
        }
    }

}

int process_commentary(struct tToken token) {
    if (!dynamic_string_cmp_const_str(token.content_string, "begin")) return 1;
    else if (!dynamic_string_cmp_const_str(token.content_string, "end")) return 2;
    else { return 3; }
}

struct tToken process_integer(struct tToken token) {
    char *arrayofchars;
    int value = strtol(token.content_string->str, &arrayofchars, 10);
    if (*arrayofchars) {
        ErrorPrint(INTERNAL_ERROR, "[scanner.c][process_integer]");
    }

    token.value.i = value;
    token.set_type_of_token = CHAR_INTEGER;
    return token;
}

struct tToken process_decimal(struct tToken token) {
    char *arrayofchars;
    double value = strtod(token.content_string->str, &arrayofchars);
    if (*arrayofchars) {
        ErrorPrint(INTERNAL_ERROR, "[scanner.c][process_decimal]");
    }

    token.value.d = value;
    token.set_type_of_token = DOUBLE;
    return token;
}

struct tToken process_identifier(struct tToken token) {
    if (!dynamic_string_cmp_const_str(token.content_string, "def")) token.set_type_of_token = KEY_WORD_DEF;
    else if (!dynamic_string_cmp_const_str(token.content_string, "if")) token.set_type_of_token = KEY_WORD_IF;
    else if (!dynamic_string_cmp_const_str(token.content_string, "while")) token.set_type_of_token = KEY_WORD_WHILE;
    else if (!dynamic_string_cmp_const_str(token.content_string, "do")) token.set_type_of_token = KEY_WORD_DO;
    else if (!dynamic_string_cmp_const_str(token.content_string, "else")) token.set_type_of_token = KEY_WORD_ELSE;
    else if (!dynamic_string_cmp_const_str(token.content_string, "not")) token.set_type_of_token = KEY_WORD_NOT;
    else if (!dynamic_string_cmp_const_str(token.content_string, "nil")) token.set_type_of_token = KEY_WORD_NIL;
    else if (!dynamic_string_cmp_const_str(token.content_string, "then")) token.set_type_of_token = KEY_WORD_THEN;
    else if (!dynamic_string_cmp_const_str(token.content_string, "end")) token.set_type_of_token = KEY_WORD_END;
    else {
        token.set_type_of_token = IDENTIFIER_NAME;
    }
    return token;
}


	



