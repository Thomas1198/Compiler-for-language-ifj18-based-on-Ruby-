/**
 * @file scanner.c
 * @author Patrik Strnad (xstrna11)
 * @brief Error codes constants and function to print errors.
 */


#include "scanner.h"



//end of includes
bool new_line = true;
int start_token = -1;

//FILE *source_file;

int process_commentary(Dynamic_string *str, struct tToken token, FILE *f, SCANNER_STATE *ptr) {
    if (!dynamic_string_cmp_const_str(str, "begin")) return 1;
    else if (!dynamic_string_cmp_const_str(str, "end")) return 2;
    else { return 3; }
}

struct tToken get_token(FILE *source_file) {
    if (source_file == NULL) {
        ErrorPrint(INTERNAL_ERROR, "[scanner.c][get_token] failed mount");
    }

    if (start_token < 1)
    {
        start_token++;
    }

    Dynamic_string *content_string;
    struct tToken token;
    init_token(&token);


    //Setuji current_state na DEFAULT
    SCANNER_STATE current_state = START;

    init_token(&token);

    content_string = dynamic_string_init();
    token.content_string = content_string;
    //Declaration of the scanner char
    int c = 0;

    while (true) {

        c = getc(source_file);

        switch (current_state) {
            case (START):

                if (c == '\n') {
                    current_state = EOL;
                    if (c == '=') {
                        ungetc(c, source_file);
                        new_line = true;
                    } else {
                        ungetc(c, source_file);
                        new_line = false;
                    }
                }

                 else if (c == '/') {
                    current_state = BACKSLASH;
                } else if (c == '#') {
                    current_state = COMMENTARY;
                } else if (c == '_' || isalpha(c)) {
                    dynamic_string_add_char(content_string, (char) tolower(c));
                    current_state = KEYWORD;
                } else if (isdigit(c)) {
                    dynamic_string_add_char(content_string, (char) tolower(c));
                    current_state = NUMBER;
                } else if (c == '<') {
                    current_state = LESS_THAN;
                } else if (c == '>') {
                    current_state = GREATER_THAN;
                } else if (c == '=') {
                     if (start_token == 0) {
                         current_state = MIGHTBECOMMENT;
                     }
                     else {
                         current_state = EQUALS;
                     }
                } else if (isspace(c)) {
                    current_state = START;
                } else if (c == '/') {
                    token.set_type_of_token = CHAR_OPERATOR_DIV;
                    dynamic_string_free(content_string);
                    return token;
                }
                else if (c == '+') {
                    token.set_type_of_token = CHAR_OPERATOR_PLUS;
                    dynamic_string_free(content_string);
                    return token;
                } else if (c == '-') {
                    token.set_type_of_token = CHAR_OPERATOR_MINUS;
                    dynamic_string_free(content_string);
                    return token;
                } else if (c == '*') {
                    token.set_type_of_token = CHAR_OPERATOR_MUL;
                    dynamic_string_free(content_string);
                    return token;
                } else if (c == ',') {
                    token.set_type_of_token = CHAR_COMMA;
                    dynamic_string_free(content_string);
                    return token;
                } else if (c == ':') {
                    current_state = MIGHTBEASSIGN;
                } else if (c == '(') {
                    token.set_type_of_token = CHAR_LEFT_BRACKET;
                    dynamic_string_free(content_string);
                    return token;
                } else if (c == ')') {
                    token.set_type_of_token = CHAR_RIGHT_BRACKET;
                    dynamic_string_free(content_string);
                    return token;
                }
                else if (c == '"') {
                    current_state = STRING_START;
                } else if (c == '!') {
                    current_state = EXCLAMATION;
                } else if (c == ';') {
                    current_state = SEMICOLON;
                } else if (c == EOF) {
                    token.set_type_of_token = CHAR_EOF;
                    dynamic_string_free(content_string);
                    return token;
                } else {
                    dynamic_string_free(content_string);
                    ErrorPrint(INTERNAL_ERROR, "[scanner.c][get_token]cannot read content of source_file"); //TODO: nemel by to byt error scanneru?
                }
                break;

            case (KEYWORD): {
                if (isalnum(c) || c == '_') {
                    dynamic_string_add_char(content_string, (char) tolower(c));
                } else {
                    ungetc(c, source_file);
                    return process_identifier(content_string, token);
                }

            }
                break;

            case (EOL):
                if (isspace(c)) {
                    break;
                }

                ungetc(c, source_file);
                token.set_type_of_token = CHAR_EOL;
                return token;

            case (EXCLAMATION):
                if (isspace(c) || (c == '\n')) {
                    token.set_type_of_token = CHAR_EXCLAMATION;
                } else if (c == '=') {
                    token.set_type_of_token = CHAR_NEQ;
                }
                return token;

            case (NUMBER):
                if (isdigit(c)) {
                    dynamic_string_add_char(content_string, (char) c);

                } else if (c == '.') {
                    current_state = NUMBER_DEC;
                } else if (tolower(c) == 'e') {
                    current_state = NUMBER_EXP;
                } else {
                    ungetc(c, source_file);
                    process_integer(content_string, token);
                }

                break;

            case (NUMBER_DEC):

                if (isdigit(c)) {
                    current_state = NUMBER_DOUBLE;
                    dynamic_string_add_char(content_string, (char) c);
                } else {
                    ungetc(c, source_file);
                    process_decimal(content_string, token);
                    //ErrorPrint(SCANNER_ERROR, "[scanner.c][get_token][NUMBER_DEC]");
                }
                break;

            case (NUMBER_DOUBLE):

                if (isdigit(c)) {
                    dynamic_string_add_char(content_string, (char) c);

                } else if (tolower(c) == 'e') {
                    current_state = NUMBER_EXP;
                    dynamic_string_add_char(content_string, (char) c);
                } else {
                    ungetc(c, source_file);
                    process_decimal(content_string, token);
                }
                break;

            case (NUMBER_EXP):
                if (isdigit(c)) {
                    current_state = NUMBER_EXP_DONE;
                    dynamic_string_add_char(content_string, (char) c);
                } else if (c == '-' || c == '+') {
                    current_state = NUMBER_EXP_SIGN;
                    dynamic_string_add_char(content_string, (char) c);
                } else {
                    dynamic_string_free(content_string);
                    ErrorPrint(SCANNER_ERROR, "[scanner.c][get_token][NUMBER_EXP]");
                }
                break;

            case (NUMBER_EXP_DONE):
                if (isdigit(c)) {
                    dynamic_string_add_char(content_string, c);
                } else {
                    ungetc(c, source_file);
                    return process_decimal(content_string, token);
                }

                break;

            case (NUMBER_EXP_SIGN):
                if (isdigit(c)) {
                    current_state = NUMBER_EXP_DONE;
                    dynamic_string_add_char(content_string, (char) c);
                } else {
                    dynamic_string_free(content_string);
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
                    dynamic_string_add_char(content_string, (char) tolower(c));
                } else if (isspace(c)) {
                    int result = process_commentary(content_string, token, source_file, &current_state);
                    if (result == 1) {
                        current_state = STARTCHUNKCOMMENTARY;
                    } else if (result == 2) {
                        current_state = ENDCHUNKCOMMENTARY;
                    } else {
                        fseek(source_file, -strlen((char *) content_string), SEEK_CUR);
                        token.set_type_of_token = CHAR_EQUALS;
                        dynamic_string_free(content_string);
                        return token;

                    }
                }
                break;


            case (ENDCHUNKCOMMENTARY): {
                current_state = START;
            }
                break;

            case (EQUALS):
                if (c == '=') {
                    current_state = MIGHTBEDOUBLE_EQ;
                }
                else {
                    token.set_type_of_token = CHAR_EQUALS;
                    dynamic_string_free(content_string);
                    return token;
                }


                break;

            case (MIGHTBECOMMENT):
                if (isalpha(c))
                {
            dynamic_string_add_char(content_string, (char) tolower(c));
                }
                else if (isspace(c))
                {
                    int result = process_commentary(content_string, token, source_file, &current_state);
                    if (result == 1)
                    {
                        current_state = STARTCHUNKCOMMENTARY;
                        dynamic_string_free(content_string);
                    }
                    else if (result == 2)
                    {
                        current_state = ENDCHUNKCOMMENTARY;
                        dynamic_string_free(content_string);
                    }
                }
                break;

            case (MIGHTBEDOUBLE_EQ):
                if (isspace(c) || isalnum(c)) {
                    token.set_type_of_token = CHAR_DOUBLEEQ;
                    ungetc(c, source_file);
                    dynamic_string_free(content_string);
                    return token;
                } else {
                    dynamic_string_free(content_string);
                    ErrorPrint(SCANNER_ERROR, "[scanner.c][get_token][MIGHTBEDOUBLE_EQ]");
                }
                break;

            case (MIGHTBEASSIGN):
                if (c == '=') {
                    token.set_type_of_token = CHAR_ASSIGN;
                    dynamic_string_free(content_string);
                    return token;
                } else {
                    dynamic_string_free(content_string);
                    ErrorPrint(SCANNER_ERROR, "[scanner.c][get_token][MIGHTBEASSIGN]");
                }
                break;

            case (STRING_START):
                if (c < 32) {
                    dynamic_string_free(content_string);
                    ErrorPrint(SCANNER_ERROR, "[scanner.c][get_token][STRING_START]");
                } else if (c == '"') {
                    dynamic_string_copy(content_string, token.content_string);

                    token.set_type_of_token = LITERAL_STRING;
                    dynamic_string_free(content_string);
                    return token;
                } else {
                    dynamic_string_add_char(content_string, (char) c);
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

            case SEMICOLON: {
                if ((isspace(c) || (c == '\n'))) {
                    token.set_type_of_token = CHAR_SEMICOLON;
                    dynamic_string_free(content_string);
                    return token;
                }

            }
                break;
        }
    }

}


struct tToken process_integer(Dynamic_string *content, struct tToken token) {
    char *arrayofchars;
    int value = strtol(content->str, &arrayofchars, 10);
    if (*arrayofchars) { //TODO: odstranil jsem asterisk
        dynamic_string_free(content);
        ErrorPrint(INTERNAL_ERROR, "[scanner.c][process_integer]");
    }

    token.value.i = value;
    token.set_type_of_token = CHAR_INTEGER;
    dynamic_string_free(content);
    return token;
}

struct tToken process_decimal(Dynamic_string *content, struct tToken token) {
    char *arrayofchars;
    double value = strtod(content->str, &arrayofchars);
    if (*arrayofchars) { //TODO: odstranil jsem asterisk
        dynamic_string_free(content);
        ErrorPrint(INTERNAL_ERROR, "[scanner.c][process_decimal]");
    }

    token.value.d = value;
    token.set_type_of_token = DOUBLE;
    dynamic_string_free(content);
    return token;
}

struct tToken process_identifier(Dynamic_string *str, struct tToken token) {
    if (!dynamic_string_cmp_const_str(str, "def")) token.set_type_of_token = KEY_WORD_DEF;
    else if (!dynamic_string_cmp_const_str(str, "if")) token.set_type_of_token = KEY_WORD_IF;
    else if (!dynamic_string_cmp_const_str(str, "while")) token.set_type_of_token = KEY_WORD_WHILE;
    else if (!dynamic_string_cmp_const_str(str, "do")) token.set_type_of_token = KEY_WORD_DO;
    else if (!dynamic_string_cmp_const_str(str, "else")) token.set_type_of_token = KEY_WORD_ELSE;
    else if (!dynamic_string_cmp_const_str(str, "not")) token.set_type_of_token = KEY_WORD_NOT;
    else if (!dynamic_string_cmp_const_str(str, "nil")) token.set_type_of_token = KEY_WORD_NIL;
    else if (!dynamic_string_cmp_const_str(str, "then")) token.set_type_of_token = KEY_WORD_THEN;
    else if (!dynamic_string_cmp_const_str(str, "end")) token.set_type_of_token = KEY_WORD_END;
    else {
        token.set_type_of_token = IDENTIFIER_NAME;
        dynamic_string_copy(str, token.content_string);
    }
    return token;
}


	



