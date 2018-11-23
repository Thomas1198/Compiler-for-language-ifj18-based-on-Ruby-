/**
 * @file scanner.c
 * @author Patrik Strnad (xstrna11)
 * @brief Error codes constants and function to print errors.
 */


#include "scanner.h"



//end of includes

Dynamic_string *content_string;
//FILE *source_file;

struct tToken get_token(FILE *source_file)
{
	if (source_file == NULL)
	{
            ErrorPrint(99,"failed mount");
	}
        
        
       struct tToken token;
	token.content_string = content_string;

	//Setuji current_state na DEFAULT
	SCANNER_STATE current_state = START;
	token.set_type_of_token = EMPTY;

	dynamic_string_init(content_string);


	//Declaration of the scanner char
	int c = 0;

	while (true)
	{
	
	c = getc(source_file);

	switch(current_state)
	{
		case (START):
		
			if (c == '\n')
			{
				current_state = EOL;
			}
			else if (c == '/')
			{
				current_state = BACKSLASH;
			}
			else if (c == '#')
			{
				current_state = CHUNK_COMMENTARY;
			}
			else if (c == '_' || isalpha(c))
			{
				dynamic_string_add_char(content_string, (char) tolower(c));
				current_state = KEYWORD;
			}
			else if (isdigit(c))
			{
				dynamic_string_add_char(content_string, (char) tolower(c));
				current_state = NUMBER;
			}
			else if (c == '<')
			{
				current_state = LESS_THAN;
			}
			else if (c == '>')
			{
				current_state = GREATER_THAN;
			}
			else if (c == '=')
			{
				current_state = EQUALS;
			}
			else if (isspace(c))
			{
				current_state = START;
			}

			else if (c == '+')
			{
				token.set_type_of_token = CHAR_OPERATOR_PLUS;
				dynamic_string_free(content_string);
				return token;
			}
			else if (c == '-')
			{
				token.set_type_of_token = CHAR_OPERATOR_MINUS;
				dynamic_string_free(content_string);
				return token;
			}
			else if (c == '*')
			{
				token.set_type_of_token = CHAR_OPERATOR_MUL;
				dynamic_string_free(content_string);
				return token;
			}
			else if (c == ',')
			{
				token.set_type_of_token = CHAR_COMMA;
				dynamic_string_free(content_string);
				return token;
			}
			else if (c == '(')
			{
				token.set_type_of_token = CHAR_LEFT_BRACKET;
				dynamic_string_free(content_string);
				return token;
			}
			else if (c == ')')
			{
				token.set_type_of_token = CHAR_RIGHT_BRACKET;
				dynamic_string_free(content_string);
				return token;
			}
			else if (c == ';')
			{
				token.set_type_of_token = CHAR_SEMICOLON;
				dynamic_string_free(content_string);
				return token;
			}

			else if (c == '"')
			{
				current_state = STRING_START;
			}

			else if (c == '!')
			{
				current_state = EXCLAMATION;
			}

			else if (c == EOF)
				{
					token.set_type_of_token = CHAR_EOF;
					dynamic_string_free(content_string);
					return token;
				}

			else
			{
				dynamic_string_free(content_string);
				ErrorPrint(99,"cannot read content of source_file");
			}
			break;
			
			//__________________

			case (EOL):
				if (isspace(c))
				{
					break;
				}

				ungetc(c, source_file);
				token.set_type_of_token = CHAR_EOF;
				return token;
			break;

			case (EXCLAMATION):
			if (isspace(c) || (c == '\n'))
			{
				token.set_type_of_token = CHAR_EXCLAMATION;
			}
			else if (c == '=')
			{
				token.set_type_of_token = CHAR_NEQ;
			}
			return token;
			break;




			case (NUMBER):
				if (isdigit(c))
				{
					dynamic_string_add_char(content_string, (char) c);
					

				}
				else if (c == '.')
				{
					current_state = NUMBER_DEC;
				}
				else if (tolower(c) == 'e')
				{
					current_state = NUMBER_EXP;
				}
				else
				{
					ungetc(c, source_file);
					//process_integer
				}

				break;

			case (NUMBER_DEC):
			
				if (isdigit(c))
				{
					current_state = NUMBER_DOUBLE;
				}
				else dynamic_string_free(content_string);

				break;

			case (NUMBER_DOUBLE):
			
				if (isdigit(c))
				{
					dynamic_string_add_char(content_string,(char) c);

				}

				else if (tolower(c) == 'e')
				{
					current_state = NUMBER_EXP;
					dynamic_string_add_char(content_string, (char) c);
				}

				else
				{
					ungetc(c, source_file);
					//process_decimal
				}
				break;

				case (NUMBER_EXP):
					if (isdigit(c))
					{
						current_state = NUMBER_EXP_DONE;
						dynamic_string_add_char(content_string, (char) c);
					}
					else if (c == '-' || c == '+')
					{
						current_state = NUMBER_EXP_SIGN;
						dynamic_string_add_char(content_string, (char) c);
					}
					else 
					{	
						dynamic_string_free(content_string);
						ErrorPrint(1,"lex error");
					}
				break;

				case (NUMBER_EXP_DONE):
				
				break;

				case (NUMBER_EXP_SIGN):
				if (isdigit(c))
				{
					current_state = NUMBER_EXP_DONE_LAST;
					dynamic_string_add_char(content_string, (char) c);
				}
				else
				{
					dynamic_string_free(content_string);
					ErrorPrint(1,"lex error");
				}
				
				break;


				case (NUMBER_EXP_DONE_LAST):
				if (isdigit(c))
				{
					dynamic_string_add_char(content_string, (char) c);
				}
				else
				{
					ungetc(c, source_file);
					//process_decimal;
				}
				break;

				case (BACKSLASH):
				if (c == '/')
				{
					current_state = BACKSLASH;
				}
				else
				{
					ungetc(c, source_file);
					token.set_type_of_token = CHAR_OPERATOR_DIV;
					dynamic_string_free(content_string);
					return token;
				}

				break;

				case (COMMENTARY):
					if (c == '\n' || c == EOF)
				{
					current_state = START;
					ungetc(c, source_file);
				}
				break;

				case (EQUALS):
				if (isalpha(c))
				{/*
					if (!dynamic_string_add_char(content_string, (char) tolower(c)))
					{
						//FSEEK VOLE
					}
                                  */
				}

				break;

				case (STRING_START):
				if (c < 32)
				{
					dynamic_string_free(content_string);
					ErrorPrint(1,"lex error");
				}
				else if (c == '"')
				{
					dynamic_string_copy(content_string, token.content_string);
					
					token.set_type_of_token = STRING;
                                        dynamic_string_free(content_string);
					return token;
				}
				else
				{
					dynamic_string_add_char(content_string, (char) c);
				}
				
				break;



			}
			}
		
	}

	struct tToken process_integer(Dynamic_string *content, struct tToken token)
	{
		char *arrayofchars;
		int value = strtol(content_string->str, &arrayofchars, 10);
		if (*arrayofchars)
		{
			dynamic_string_free(content_string);
			ErrorPrint(99,"stringadd error");
		}

		token.value = (union value_union) value;
		token.set_type_of_token = INTEGER;
		dynamic_string_free(content_string);
		return token;
	}

	struct tToken process_decimal(Dynamic_string *content, struct tToken token)
	{
		char *arrayofchars;
		int value = strtod(content_string->str, &arrayofchars);
		if (*arrayofchars)
		{
			dynamic_string_free(content_string);
			ErrorPrint(99,"stringadd error");
		}

		token.value = (union value_union) value;
		token.set_type_of_token = DOUBLE;
		dynamic_string_free(content_string);
		return token;
	}

	struct tToken process_identifier(Dynamic_string *str, struct tToken token)
	{
            /*
		if (!dynamic_string_cmp_const_str(str, "int")) token.set_type_of_token = KEY_WORD_INT;
		else if (!dynamic_string_cmp_const_str(str, "if")) token.set_type_of_token = KEY_WORD_IF;
		//else if (!dynamic_string_cmp_const_str(str, "for")) token.set_type_of_token = KEY_WORD_FOR;
		else if (!dynamic_string_cmp_const_str(str, "while")) token.set_type_of_token = KEY_WORD_WHILE;
		//else if (!dynamic_string_cmp_const_str(str, "bool")) token.set_type_of_token = KEY_WORD_BOOL;
		//else if (!dynamic_string_cmp_const_str(str, "true")) token.set_type_of_token = KEY_WORD_TRUE;
		//else if (!dynamic_string_cmp_const_str(str, "false")) token.set_type_of_token = KEY_WORD_FALSE;
		else if (!dynamic_string_cmp_const_str(str, "do")) token.set_type_of_token = KEY_WORD_DO;
		//else if (!dynamic_string_cmp_const_str(str, "exit")) token.set_type_of_token = KEY_WORD_EXIT;
		else if (!dynamic_string_cmp_const_str(str, "else")) token.set_type_of_token = KEY_WORD_ELSE;
		//else if (!dynamic_string_cmp_const_str(str, "elseif")) token.set_type_of_token = KEY_WORD_ELSEIF;
		//else if (!dynamic_string_cmp_const_str(str, "double")) token.set_type_of_token = KEY_WORD_DOUBLE;
		else if (!dynamic_string_cmp_const_str(str, "end")) token.set_type_of_token = KEY_WORD_END;
                */
	}


	



