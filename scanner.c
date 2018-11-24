/**
 * @file scanner.c
 * @author Patrik Strnad (xstrna11)
 * @brief Error codes constants and function to print errors.
 */


#include "scanner.h"



//end of includes

Dynamic_string *content_string;
//FILE *source_file;

struct tToken get_token(FILE *source_file) {
    if (source_file == NULL) {
        ErrorPrint(99, "failed mount");
    }


    struct tToken token;
    token.content_string = content_string;

    //Setuji current_state na DEFAULT
    SCANNER_STATE current_state = START;
    token.set_type_of_token = UNDEFINED_SET;

    content_string = dynamic_string_init();

    //Declaration of the scanner char
    int c = 0;

    while (true) {

        c = getc(source_file);

        switch (current_state) {
            case (START):

                if (c == '\n') {
                    current_state = EOL;
                } else if (c == '/') {
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
                    current_state = EQUALS;
                } else if (isspace(c)) {
                    current_state = START;
                } else if (c == '+') {
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
                } else if (c == '(') {
                    token.set_type_of_token = CHAR_LEFT_BRACKET;
                    dynamic_string_free(content_string);
                    return token;
                } else if (c == ')') {
                    token.set_type_of_token = CHAR_RIGHT_BRACKET;
                    dynamic_string_free(content_string);
                    return token;
                }
                /*else if (c == ';') {
                    token.set_type_of_token = CHAR_SEMICOLON;
                    dynamic_string_free(content_string);
                    return token;
                }
                 */
                else if (c == '"') {
                    current_state = STRING_START;
                } else if (c == '!') {
                    current_state = EXCLAMATION;
                }
                else if (c == ';') {
                    current_state = SEMICOLON;
                }
                else if (c == EOF) {
                    token.set_type_of_token = CHAR_EOF;
                    dynamic_string_free(content_string);
                    return token;
                } else {
                    dynamic_string_free(content_string);
                    ErrorPrint(99, "cannot read content of source_file");
                }
                break;

            case (KEYWORD):
            {
                if (isalnum(c) || c == '_')
                {
                    dynamic_string_add_char(content_string, (char) tolower(c));
                }
                else
                {
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
                } else dynamic_string_free(content_string);

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
                    ErrorPrint(1, "lex error");
                }
                break;

            case (NUMBER_EXP_DONE):

                break;

            case (NUMBER_EXP_SIGN):
                if (isdigit(c)) {
                    current_state = NUMBER_EXP_DONE_LAST;
                    dynamic_string_add_char(content_string, (char) c);
                } else {
                    dynamic_string_free(content_string);
                    ErrorPrint(1, "lex error");
                }

                break;


            case (NUMBER_EXP_DONE_LAST):
                if (isdigit(c)) {
                    dynamic_string_add_char(content_string, (char) c);
                } else {
                    ungetc(c, source_file);
                    process_decimal(content_string, token);
                }
                break;

            case (BACKSLASH):
                if (c == '/') {
                    current_state = BACKSLASH;
                } else {
                    ungetc(c, source_file);
                    token.set_type_of_token = CHAR_OPERATOR_DIV;
                    dynamic_string_free(content_string);
                    return token;
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
                    if (isalpha(c)) {
                        dynamic_string_add_char(content_string, (char) tolower(c));
                    } else if (isspace(c)) {
                        return process_commentary(content_string, token, source_file, &current_state);
                    }
                }
            }
                break;

            case (ENDCHUNKCOMMENTARY): {
                current_state = START;
            }
                break;

            case (EQUALS):
                if (isalpha(c)) {
                    dynamic_string_add_char(content_string, (char) tolower(c));
                } else if (isspace(c)) {
                    return process_commentary(content_string, token, source_file, &current_state);
                }

                break;

            case (STRING_START):
                if (c < 32) {
                    dynamic_string_free(content_string);
                    ErrorPrint(1, "lex error");
                } else if (c == '"') {
                    dynamic_string_copy(content_string, token.content_string);

                    token.set_type_of_token = LITERAL_STRING;
                    dynamic_string_free(content_string);
                    return token;
                } else {
                    dynamic_string_add_char(content_string, (char) c);
                }

                break;


            case LESS_THAN:
            {
                if (c == '=')
                {
                    token.set_type_of_token = CHAR_LEQ;
                }
                else
                {
                    ungetc(c, source_file);
                    token.set_type_of_token = CHAR_LT;
                }

            }  break;
            case GREATER_THAN:
            {
                if (c == '=')
                {
                    token.set_type_of_token = CHAR_GEQ;
                }
                else
                {
                    ungetc(c, source_file);
                    token.set_type_of_token = CHAR_GT;
                }

            }
            break;

            case SEMICOLON:
            {
                if((isspace(c) || (c=='\n')))
                {
                    token.set_type_of_token = CHAR_SEMICOLON;
                    dynamic_string_free(content_string);
                    return token;
                }

            }
            break;
        }
    }

}

struct tToken process_commentary(Dynamic_string *str, struct tToken token, FILE *f, SCANNER_STATE *state) {
    if (!dynamic_string_cmp_const_str(str, "=begin")) *state = STARTCHUNKCOMMENTARY;
    else if (!dynamic_string_cmp_const_str(str, "=end")) *state = ENDCHUNKCOMMENTARY;
    else {
        if (*state != STARTCHUNKCOMMENTARY) {
            fseek(f, strlen((char*)str) - 1, SEEK_CUR);
            token.set_type_of_token = EQUALS;
            dynamic_string_free(content_string);
            return token;
        }
    }
}

struct tToken process_integer(Dynamic_string *content, struct tToken token) {
    char *arrayofchars;
    int value = strtol(content->str, &arrayofchars, 10);
    if (*arrayofchars) {
        dynamic_string_free(content);
        ErrorPrint(99, "stringadd error");
    }

    token.value.i =  value;
    token.set_type_of_token = CHAR_INTEGER;
    dynamic_string_free(content);
    return token;
}

struct tToken process_decimal(Dynamic_string *content, struct tToken token) {
    char *arrayofchars;
    double value = strtod(content->str, &arrayofchars);
    if (*arrayofchars) {
        dynamic_string_free(content);
        ErrorPrint(99, "stringadd error");
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
    else token.set_type_of_token = IDENTIFIER_NAME;

    return token;
}


	



